## Description

Project Name : TCPFighterServer

Language : C++14

Platform : Windows11 x86

IDE : Visual Studio 2022

Update Date : 2026-06-07

```text
Network
 ├─ Session
 ├─ SessionManager
 ├─ Packet
 ├─ RingBuffer
 ├─ MemoryPool
 └─ RPC

IOCP Network Library 개발
커스텀 패킷 직렬화
RPC + 자동화
클라이언트는 먼저 액션(이동, 공격 ...)을 하고 패킷을 서버로 보냄
클라이언트로 공격쿨 전에 메세지가 들어오면 이펙트 스킵 / 체력만 감소
충돌 처리 및 데미지에 대한 정보는 서버에서 처리 후 통보
```

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

### 26-06-15
```
프로파일러 단축키 제대로 입력이 씹힘 -> 여러 스레드에서 getch() 호출해서 생긴 문제
-> 키보드 입력은 메인스레드에서만 가능토록 수정


```
