# TCPGameServer

C++ 기반의 TCP 게임 서버 프로젝트입니다.

IOCP를 기반으로 네트워크 라이브러리를 직접 구현하고, 커스텀 바이너리 패킷 프로토콜과 IDL 기반 RPC 코드 자동화를 적용했습니다.

클라이언트와 서버를 하나의 솔루션에서 개발하며, 네트워크 계층과 컨텐츠 계층을 분리하여 게임 로직이 TCP 스트림 및 소켓을 직접 다루지 않도록 구성했습니다.

## Project Information

| 항목       | 내용                            |
| -------- | ----------------------------- |
| Project  | TCPFighterServer              |
| Language | C++14                         |
| Platform | Windows 11 x86                |
| IDE      | Visual Studio 2022            |
| Client   | C++, ImGui, DirectX 11, Win32 |
| Database | MySQL                         |
| Network  | TCP / IOCP                    |

## Architecture

전체 서버는 네트워크 처리, 게임 로직, 데이터베이스 처리를 분리하는 구조로 구성했습니다.

```text
                    Client
                      │
                      │ TCP
                      ▼
              ┌───────────────┐
              │   LanServer   │
              │     IOCP      │
              └───────┬───────┘
                      │
              Packet 단위 전달
                      │
                      ▼
              ┌───────────────┐
              │ Network Queue │
              └───────┬───────┘
                      │
                      ▼
              ┌───────────────┐
              │  Logic Thread │
              │   MyServer    │
              └───────┬───────┘
                      │
             DB Request Queue
                      │
                      ▼
              ┌───────────────┐
              │   DB Thread   │
              │     MySQL     │
              └───────┬───────┘
                      │
             DB Response Queue
                      │
                      ▼
                 Logic Thread
```

### Network Layer

`LanServer`는 IOCP 기반의 네트워크 라이브러리로, 컨텐츠 코드가 소켓과 `OVERLAPPED` 등의 네트워크 세부 구현을 직접 다루지 않도록 추상화했습니다.

컨텐츠 서버는 `LanServer`를 상속받아 필요한 이벤트를 구현합니다.

```cpp
class MyServer : public LanServer,
                 public RpcServerHandler,
                 public DatabaseServerHandler
{
private:
    bool OnConnectionRequest(
        const std::wstring& ip,
        int port) override;

    void OnAccept(__int64 sessionId) override;
    void OnRelease(__int64 sessionId) override;
    void OnRecv(
        __int64 sessionId,
        Packet* packet) override;
};
```

네트워크 계층에서는 `Session`을 관리하며, 컨텐츠 계층에는 소켓이나 `Session` 객체를 직접 노출하지 않고 `sessionId`를 통해 세션을 식별하도록 구성했습니다.

```text
Network Layer
    │
    │ sessionId
    ▼
Content Layer
```

이를 통해 컨텐츠 코드는 TCP 연결 및 IOCP의 동작 방식에 의존하지 않고 패킷 단위의 메시지만 처리합니다.

---

## IOCP Network Library

Windows IOCP를 기반으로 TCP 네트워크 라이브러리를 구현했습니다.

### 주요 구성

* IOCP 기반 비동기 TCP 서버
* Accept Thread / Worker Thread 분리
* `WSARecv` / `WSASend` 기반 Overlapped I/O
* Session 관리
* Session별 동기화
* Send / Receive Ring Buffer
* IO Count 기반 Session 수명 관리
* Packet 단위 Receive 처리
* Send Queue를 이용한 비동기 송신
* Session ID 기반 외부 인터페이스

### Receive Flow

TCP는 스트림 기반이기 때문에 한 번의 `recv`가 하나의 패킷이라는 보장이 없습니다.

따라서 네트워크 계층에서 수신 데이터를 Ring Buffer에 저장한 후 패킷 헤더의 크기를 기준으로 완전한 패킷을 조립합니다.

```text
TCP Stream
    │
    ▼
Receive RingBuffer
    │
    ├── incomplete packet
    │       └── wait for next receive
    │
    └── complete packet
            │
            ▼
        Packet 생성
            │
            ▼
        OnRecv()
```

컨텐츠 계층에서는 TCP 데이터가 여러 번 나누어져 들어왔는지 여부를 알 필요 없이 항상 완성된 `Packet` 단위로 전달받습니다.

```cpp
void MyServer::OnRecv(__int64 sessionId, Packet* packet)
{
    packet->SetId(sessionId);

    // Logic Thread에서 처리하기 위해 Queue에 전달
    networkPacketQueue_.Push(packet);
}
```

IOCP Worker Thread에서 컨텐츠 로직을 직접 실행하지 않고 Logic Thread로 전달하기 때문에, 컨텐츠 코드는 단일 Logic Thread를 기준으로 동작합니다.

---

## Thread Model

컨텐츠 로직은 하나의 Logic Thread에서 처리하도록 구성했습니다.

```text
IOCP Worker Threads
        │
        │ Packet
        ▼
Network Packet Queue
        │
        ▼
   Logic Thread
        │
        ├───────────────┐
        │               │
        ▼               ▼
   Game Logic      DB Request Queue
                        │
                        ▼
                    DB Thread
                        │
                        ▼
                  MySQL Database
```

### Network Layer

네트워크 라이브러리에서는 여러 Worker Thread가 동일한 Session에 접근할 수 있기 때문에 Session 단위의 Lock을 사용하여 동기화합니다.

### Content Layer

컨텐츠 로직은 Logic Thread 하나에서만 실행됩니다.

따라서 다음과 같은 컨텐츠 데이터는 별도의 Lock 없이 접근할 수 있도록 설계했습니다.

```cpp
std::unordered_map<__int64, int> sessionToPlayer_;
std::unordered_map<int, Player*> playerMap_;
```

이를 통해 컨텐츠 코드에서는 불필요한 동기화를 최소화하고, 네트워크 계층에서 필요한 동기화와 컨텐츠 로직의 실행 순서를 분리했습니다.

---

# Custom Binary Packet

커스텀 바이너리 패킷 프로토콜을 구현했습니다.

패킷은 다음과 같은 Header와 Payload 구조를 사용합니다.

```cpp
struct MY_HEADER
{
    short size_;
    short type_;
};
```

```text
┌──────────────┬──────────────┬──────────────────┐
│ size (2byte) │ type (2byte) │     payload      │
└──────────────┴──────────────┴──────────────────┘
```

`size_`를 이용해 TCP 스트림에서 하나의 패킷을 구분하고, `type_`을 이용해 패킷 종류를 구분합니다.

## Serialization

`Packet` 객체에 `operator<<`, `operator>>`를 구현하여 패킷 직렬화와 역직렬화를 처리합니다.

```cpp
Packet& operator<<(char value)
{
    *(char*)(buffer_ + writePos_) = value;
    writePos_ += sizeof(char);

    return *this;
}
```

사용하는 쪽에서는 타입별 직렬화 구현을 직접 호출하지 않고 다음과 같이 사용할 수 있습니다.

```cpp
*packet << loginId << password;
```

역직렬화 역시 동일한 인터페이스를 사용합니다.

```cpp
*packet >> loginId >> password;
```

이를 통해 패킷 송수신 코드에서 버퍼의 위치를 직접 관리하는 코드를 줄였습니다.

---

# RPC

IDL을 기반으로 Client / Server RPC 코드를 자동 생성하는 구조를 구현했습니다.

RPC 정의는 별도의 IDL 파일에서 관리합니다.

```text
ReqUserRegister(std::string& loginId, std::string& password)    0
ResUserRegister(RESPONSE_CODE code)                              1
```

IDL을 기반으로 다음 코드가 생성됩니다.

```text
             RPC IDL
                │
                ▼
          RPC Generator
                │
        ┌───────┴───────┐
        ▼               ▼
   Client Proxy      Server Stub
        │               │
        ▼               ▼
   Serialization    Deserialization
        │               │
        └───────┬───────┘
                ▼
              Packet
```

## Server Proxy

서버에서 클라이언트로 패킷을 보내는 코드를 Proxy가 담당합니다.

```cpp
void RpcServerProxy::ReqUserRegister(
    __int64 sessionId,
    std::string& loginId,
    std::string& password)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->GetHeaderPtr()->type_ = 0;
    *packet << loginId << password;

    server_->SendPacket(sessionId, packet);
}
```

## Client Stub

수신한 패킷은 Stub에서 `type_`을 기준으로 역직렬화한 후 Handler를 호출합니다.

```cpp
bool RpcClientStub::PacketProc(Packet* packet)
{
    switch (packet->GetHeaderPtr()->type_)
    {
        case 0:
        {
            std::string loginId;
            std::string password;

            *packet >> loginId >> password;

            return handler_->ReqUserRegister(
                loginId,
                password);
        }
    }

    return false;
}
```

실제 컨텐츠 동작은 Handler에서 구현합니다.

```cpp
bool RpcServerHandler::ReqUserRegister(
    __int64 sessionId,
    std::string& loginId,
    std::string& password)
{
    // Content Logic
    return true;
}
```

따라서 컨텐츠 개발자는 패킷 ID, 직렬화, 역직렬화 및 패킷 타입 분기 코드를 직접 작성할 필요 없이 RPC Handler를 구현하는 방식으로 메시지 처리를 추가할 수 있습니다.

---

# Database Processing

DB 접근 역시 별도의 DB Thread에서만 수행하도록 구성했습니다.

```text
Logic Thread
    │
    │ DB Request
    ▼
DB Request Queue
    │
    ▼
DB Thread
    │
    │ MySQL Query
    ▼
MySQL
    │
    ▼
DB Thread
    │
    │ DB Response
    ▼
DB Response Queue
    │
    ▼
Logic Thread
```

MySQL 접근은 MySQL Connector/C++를 사용하며, DB Thread 이외의 스레드에서는 직접 쿼리를 실행하지 않습니다.

## DB Proxy / Stub

DB 요청 및 응답 역시 RPC와 유사한 Proxy / Stub 구조로 자동화했습니다.

차이점은 RPC가 네트워크를 통해 패킷을 전달한다면, DB RPC는 DB Thread와 Logic Thread 사이의 Queue를 통해 전달한다는 점입니다.

```cpp
void DatabaseServerProxy::ReqUserRegisterDB(
    __int64 sessionId,
    std::string& loginId,
    std::string& password)
{
    Packet* packet = new Packet();
    packet->Initialize();

    packet->SetId(sessionId);
    packet->GetHeaderPtr()->type_ = 0;

    *packet << loginId << password;

    dbQueue_->Push(packet);
}
```

DB Thread에서는 Queue에서 요청을 가져와 Stub을 통해 Handler를 호출합니다.

```cpp
bool DatabaseServerStub::DbPacketProc(
    __int64 sessionId,
    Packet* packet)
{
    switch (packet->GetHeaderPtr()->type_)
    {
        case 0:
        {
            std::string loginId;
            std::string password;

            *packet >> loginId >> password;

            return handler_->ReqUserRegisterDB(
                sessionId,
                loginId,
                password);
        }
    }

    return false;
}
```

---

# Memory First Data Management

게임 서버의 런타임 데이터는 가능한 경우 DB를 직접 조회하지 않고 서버 메모리를 기준으로 처리하도록 구성했습니다.

예를 들어 플레이어의 골드가 변경되는 경우:

```text
Client Request
      │
      ▼
Logic Thread
      │
      ├── Memory Update
      │       │
      │       ▼
      │   Client Response
      │
      └── DB Update Request
              │
              ▼
          DB Thread
              │
              ▼
             MySQL
```

게임 플레이 중 발생하는 모든 상태 변경마다 DB를 동기적으로 조회하지 않고, 서버 메모리에 반영한 후 클라이언트에 결과를 전달합니다.

DB에는 최종 상태를 비동기적으로 저장합니다.

반대로 서버가 시작되거나 플레이어가 처음 로비에 진입하는 등 메모리에 데이터가 없는 경우에는 DB에서 데이터를 조회한 후 Logic Thread로 결과를 전달하여 서버 메모리에 적재합니다.

```text
First Login / Lobby Enter

Logic Thread
     │
     ▼
DB Request
     │
     ▼
DB Thread
     │
     ▼
MySQL SELECT
     │
     ▼
DB Response Queue
     │
     ▼
Logic Thread
     │
     ├── Update Server Memory
     └── Send Player Data
```

이 구조를 통해 게임 플레이 중 DB 접근에 의존하지 않고 서버 메모리를 중심으로 게임 상태를 처리합니다.

---

# Session / Player Management

클라이언트가 전달하는 `playerId`를 서버에서 직접 신뢰하지 않고, 서버가 관리하는 `sessionId`와 Player를 매핑하여 사용합니다.

```text
Session ID
    │
    ▼
sessionToPlayer_
    │
    ▼
Player ID
    │
    ▼
playerMap_
    │
    ▼
Player
```

```cpp
std::unordered_map<__int64, int> sessionToPlayer_;
std::unordered_map<int, Player*> playerMap_;
```

따라서 클라이언트가 임의의 Player ID를 패킷에 포함하여 요청하더라도 서버는 현재 연결된 Session과 매핑된 Player를 기준으로 처리합니다.

---

# Current Content

현재 구현된 컨텐츠입니다.

### Implemented

* 회원가입
* 로그인
* 로비
* 플레이어 정보

  * 이름
  * 레벨
  * 골드
* 캐릭터 보유 목록
* 전체 채팅
* 게임 접속 중 유저 목록

### Planned

* 상점
* 게임 매치 진입
* 게임 플레이
* 게임 종료 및 결과 처리
* 게임 플레이 관련 컨텐츠

---

# Client

클라이언트는 C++ 기반으로 구현했으며 ImGui를 사용하여 게임 UI를 구성했습니다.

DirectX 11 및 Win32 기반의 예제 구조를 바탕으로 클라이언트를 구현하고, 실제 서버와 TCP 통신을 수행하도록 구성했습니다.

```text
Client
├── Win32
├── DirectX 11
├── ImGui
└── TCP Client
       │
       │ Custom Packet
       ▼
    Game Server
```

클라이언트와 서버는 동일한 RPC 정의를 기반으로 통신하도록 구성하여 양쪽에서 동일한 메시지 규약을 사용합니다.

---

# Project Structure

```text
NetworkLibrary
│
├── NetworkLibrary
│   ├── LanServer
│   ├── Session
│   ├── Packet
│   ├── RingBuffer
│   └── ...
│
├── RPCGenerator
│   └── IDL → RPC Proxy / Stub
│
└── TCPFighterServer
    ├── Server
    ├── Client
    ├── Database
    ├── Repository
    └── Content
```

실제 프로젝트에서는 네트워크 라이브러리와 컨텐츠 서버를 분리하여, 네트워크 계층이 특정 게임 컨텐츠에 의존하지 않도록 구성했습니다.

---

# Technical Summary

## Network

* Windows IOCP
* Overlapped I/O
* TCP
* Accept Thread / Worker Thread
* Session Management
* Session-level Synchronization
* Send / Receive Ring Buffer
* Packet Framing

## Packet

* Custom Binary Protocol
* Fixed Header
* Packet Type
* Serialization / Deserialization
* `operator<<` / `operator>>`

## RPC

* Custom IDL
* RPC Code Generator
* Client Proxy / Stub
* Server Proxy / Stub
* Automatic Packet ID Management
* Serialization / Deserialization Code Generation

## Server

* Logic Thread 기반 컨텐츠 처리
* Network Queue
* Session ↔ Player Mapping
* Memory 중심 게임 상태 관리

## Database

* MySQL
* MySQL Connector/C++
* Dedicated DB Thread
* DB Request / Response Queue
* DB Proxy / Stub
* Asynchronous Persistence

## Client

* C++
* Win32
* DirectX 11
* ImGui

---

# Development Environment

```text
Language  : C++14
Platform  : Windows 11 x86
IDE       : Visual Studio 2022

Network   : TCP / IOCP
Database  : MySQL
Client UI : ImGui
Graphics  : DirectX 11
```

# Project Goal

단순한 게임 기능 구현보다는 게임 서버 개발에 필요한 네트워크 계층을 직접 구현하고, 그 위에 RPC 및 DB 처리 시스템을 구성하여 서버 구조 전반을 이해하는 것을 목표로 개발하고 있습니다.

현재는 네트워크 라이브러리와 RPC/DB 처리 구조를 기반으로 로비 및 플레이어 관련 컨텐츠를 구현했으며, 이후 상점 및 실제 게임 플레이 영역까지 확장할 예정입니다.




## DevLog
### 이전
```text
문제 발생 -> 원인 완벽하게 파악 (디버깅, 로그, ...) -> 해결

지연삭제 -> 이터레이터로 인한 문제 방지. 즉 배열은 적용 x

accept thread 분리, accept() 블로킹 호출 why? 쓸데없이 도는 코드는 절대 없어야함

50byte Sleep(1) 기준 wsasend 두번 호출 vs wsasend 한번에 wsabuf 2개등록
83us / 7us
-> 당연히 시스템콜 호출이 적은 후자가 성능 좋음
```

### 26-06-05
```text
MemoryPool 클래스에서 Node를 malloc으로 할당하므로 생성자 호출 안됨 주의
ex) struct Node {int *data = nullptr;} -> malloc 시 nullptr 대입 안됨!

RinBuffer, Packet의 실제 버퍼는 반드시 buffer 메서드로 가져와야 함
RingBuffer에 Packet을 전달해서 디큐한 다음에는 Packet의 쓰기포지션 이동

먼저 워커스레드 1개로 실험
더미테스트의 oversendcount 옵션 설정시 문제 발생
발생원인은, sendPacket()이 여러번 호출되어 registerSend()에서 완료가 되지 않은 overlapped 객체가 초기화됨
-> sendPending 변수 도입
다른 스레드에서 sendPacket 호출할 가능성 존재 -> pending 변수를 인터락으로 관리

closesocket 이후, 같은 소켓이 생성되어도, 어차피 sendPacket()은 sessionId를 기준으로 진행되어서 문제 발생 x
```

### 26-06-06
```text
폴더명 변경시 vcxproj 파일이 업데이트가 안되어서 문제 발생 가능 -> 새 프로젝트 만들어서 옮기기
이전 select모델 프로젝트 코드들(rpc, 컨텐츠 코드, 유틸 등) 추가

세션 종료시 postSend()에서 접근 못하게 ioCount 확인하는 코드 추가

발생 가능한 이슈
1. postSend() 내부에서 sendQueue 읽기에 락 필요? 다른스레드에서 sendQueue에 쓰기가 상황이 영향을 끼칠지??
2. 여러 스레드에서 세션맵 건들시 문제 발생 가능(세션 생성 삭제시 여러 스레드에서 접근 가능) -> 동기화 필요
3. sendPacket()은 동기화 필요. 다른 스레드에서 enqueue 가능 (게임서버니까)
```

### 26-06-07
```text
Logger에 날짜, 스레드, 로그타입 추가 (spdlog 참고)

server 예외처리 추가 + PQCS 적용
스레드 생성하자마자 핸들 닫으면 핸들 무효화됨 주의
커널 오브젝트의 reference count와 signal 상태는 별개의 개념임
GQCS 반환시 numOfBytes 체크 필요 x (어차피 에러 확인 코드 있으니깐)

windows.h -> 반드시 winsock 밑에!! 헤더 include 하면서 순서 꼬일수 있음 주의
#define WIN32_LEAN_AND_MEAN 추가도 가능 but windows.h의 헤더들이 많이 날라감..
```

### 26-06-08
```text
select 모델 삭제 (레포지토리 분리)

WSARecv()시 등록과 완료시 ioCount 증감 적용
ref count는 한 작업이 종료될때까지 들고 있다가 완료되면 감소. 0이 되면 삭제.
postSend()는 WSASend() 호출 전에 iocount 늘려야
다른 스레드에서 send 완료통지로 ioCount 내려서 0이 되는 상황 방지
```

### 26-06-09
```text
Server 클래스의 소멸자 로직의 로그 출력이 안되는 상황 발생 (싱글톤으로 인한 소멸 순서 미보장)
->selectserver의 소멸자 로직을 별도의 종료함수로 분리

더미테스트
클라 1 / 오버센드 1 / 연결종료 x / 딜레이 0 / TPS 약 20000
어느 시점 갑자기 TPS 0으로 떨어짐. 서버에선 세션 삭제 문구 뜨지 않음
워커스레드 종료는 없음 확인
클라 서버 둘다 소켓 삭제 안되었는데 데이터 전송은 없음
wsasend() error  10022 WSAEINVAL 이 에러가 발생하는걸로 보아 overlapped 객체가 사용중인데
덮어씌워지는 상황인거 같긴한데, 이 에러가 항상 출력되는건 아님.. 이유가??

sendQueue에 동시에 enqueue하는 상황이 문제 -> 세션 멤버변수에 Lock 추가
세션의 모든 멤버변수에 대한 접근에 락 적용
```

### 26-06-10
```text
sendQueue에 enqueue할때 락을 적용 안하면 wsasend() 10022 에러 발생
sendQueue 내부적으로 락을 적용한다고 해도, 결국 enqueue 호출부분이 2번이라서 문제가 발생 가능함... 이걸 어케 해결함?
```

### 26-06-11
```text
세션맵 잠금 없을때 더미에서 재연결 옵션시 read access violation 발생
상대가 closesocket()시 (rst x) GQCS 성공 반환 후 numOfBytes 0 전달됨 -> 예외처리

로그 전부 주석처리. 출력시간으로 인하여 동기화 문제가 안타나남

더미 클라 1로 테스트
lock을 포인터로 분리했음에도 recv /send 에서 10053 에러 발생중임...
락을 릴리즈 한 후 재사용해서 발생하는 문제임
```

### 26-06-12
```text
Server, SessionManager 클래스 -> LanServer 클래스 전면수정
LanServer 클래스는 상속이 필요하므로 싱글턴 제거

링버퍼에 락 추가

Fighter Server IOCP로 재설계중...
```

### 26-06-13
```text
config 파일 및 클래스 수정 (전역변수 방식으로 변경)

링버퍼의 useSize() 메서드에서 동기화 문제로 음수가 반환되는 문제 발생
쓰기 위치 / 읽기 위치를 메서드 진입시 스택으로 빼고 진행

FighterServer 수정 완료 (LanServer 인터페이스에 맞게)
RPC 코드에서 패킷 타입 관리하도록 수정
```

### 26-06-15
```text
Session 클래스 삭제, 구조체로 변경, 메서드는 LanServer 클래스로 편입
LanServer의 변수를 기존 클래스의 내의 전역변수에서 
메인함수의 스택에서 선언하도록 위치 변경
사용자측의 라이브러리 사용 복잡성 해소를 위함

RPC에서 Server 인스턴스 접근 방법 고민중...

Session* LanServer::findSession(__int64 sessionId)
{
	sessionMapLock_.lock();

	auto it = sessionMap_.find(sessionId);
	auto end = sessionMap_.end();

	sessionMapLock_.unlock();

	if (it != end)
	{
		return (*it).second;
	}
	else
		return nullptr;
}

위 코드 삭제
세션맵락 언락 후 세션이 삭제될 가능성 존재

RPC Proxy Stub 클래스는 기존의 싱글턴 방식에서, 
LanServer를 상속받는 클래스에서 하나씩 포함하도록 수정
```

### 26-06-16
```
프로파일러 단축키 제대로 입력이 씹힘 -> 여러 스레드에서 getch() 호출해서 생긴 문제
-> 키보드 입력은 메인스레드에서만 가능토록 수정
```

### 26-06-17
```
컨텐츠 서버 내부에서 모든 컨텐츠 관리(PlayerManager, 로직스레드)

가상함수 defaultStart(), defaultStop() 추가 (컨텐츠 서버 구현사항)

RPC 코드 서버/클라이언트(미구현)으로 분리
컨텐츠서버 인스턴스당 proxy는 멤버변수로, stub은 상속받도록 수정

onaccept() 와 postRecv() 순서의 문제?...
(완료통지가 안오는 문제현상 발생했었음.)
```

### 26-06-19
```
send 링버퍼를 Packet 포인터 저장하게 변경 -> 복사 생략 효과

WSASend() 호출시 wsabuf count 0인 상황 발생시 10022 에러 발생함. 예외처리

링버퍼 enqueue시 프리사이즈 0으로 뜨는 상황...?
-> for문 탈출조건에 어떤 메서드 넣지 말고 스택변수로 넣기 & 원형큐 크기 충분히 늘리기

Packet 클래스 buffer_ 앞쪽은 헤더가 사용하게끔 전체 수정
-> 즉 wsasend() 호출시, Packet 인스턴스의 전체 버퍼 전달하도록 함
cf. 이전의 다른 방식
1. 헤더도 Packet 인스턴스 생성 -> 생성해제 비용 and Packet 인스턴스라기엔 모호
2. 패킷 클래스 안에 헤더 멤버변수 추가 -> wsabuf에 헤더와 패킷 따로 등록해야 해서 비용 증가
```

### 26-08-01
```
재귀락 

세션맵 락 - 세션 락으로 인한 데드락 수정
세션맵 언락을 중간이 아닌 함수 마지막으로 보냄
sendQ를 처리하는 과정에서는 session접근시 모두 세션맵락 -> 세션락 순서 적용

모니터링 추가 (TPS)

메서드 네이밍 컨벤션 교체 (파스칼 케이스)
```

### 26-08-05
```
send시 복사 생략 로직 수정

Packet 클래스 수정

프로파일러 멀티스레드용 추가
```

### 26-08-16
```
LanClient 작성 (단일 세션 기반)
TestClient 추가

소켓 에러 함수 LanServer로 이동
```

### 26-08-17
```
Packet 수정 (SPacket RPacket Message 클래스 삭제)
Packet 클래스는 이제 기본적으로 헤더와 세션 id를 포함하고 있음

Session 생성시 ioCount 1로 시작.
IncrementIoCount() 수정 -> count 0에서는 증가하지 못하도록
여러 스레드에서 접근시, ReleaseSession()이 여러번 호출되는 문제 발생했었음
```

### 26-08-18
```
Rpc 코드 전면 수정 및 배치파일과 idl 파일 경로 수정
Rpc 클라이언트용 코드 추가 (sessionId를 고려하지 않기 떄문)
string, list 타입 추가
Rpc코드를 헤더파일로만 작동하도록 수정했다가 롤백 (RpcModule 코드를 사용할수 없게되어서 불가능함)
Rpc 인자는 레퍼런스 적용 가능하게 수정

stub 코드와 handler 코드 재분리
```

### 26-08-20
```
DB 구조 추가
```

### 26-08-24
```
users, players 테이블 추가
```

### 26-08-27
```
클라이언트 추가
```
