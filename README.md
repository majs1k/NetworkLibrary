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

Network Library 개발중
select IO 모델 / IOCP 모델 분리
패킷 직렬화
RPC + 자동화?
클라이언트는 먼저 액션(이동, 공격 ...)을 하고 패킷을 서버로 보냄
클라이언트로 공격쿨 전에 메세지가 들어오면 이펙트 스킵 / 체력만 감소
충돌 처리 및 데미지에 대한 정보는 서버에서 처리 후 통보
```

## TODO
```text
DB 연동
메세지 쿨타임 처리 (3번 허용 후 종료)
c# -> NetworkStream + rpc 코드
라이브러리 64비트 호환
```

## Question
```text
예외처리 try-catch?

라이브러리의 헤더파일에 define 값을 맘대로 바꿀수 있나?

메모리 풀 관련
메모리 릭(custom new-delete)/오버플로우(no access page) 라이브러리 활용도?
placement new 활용도??

로그에서 락 걸고 큐에 담은후 1초에 한번씩 flush 하는 경우 - 문제 발생 x?


sendPacket에서 헤더를 넣어야 한다면.. 타입은?
RPCProxy -> 인자가 sessionId?
헤더랑 메세지를 같이 packet에 넣은 다음 sendPacket 호출중

생성자 소멸자에는 락 적용???

서버쪽에서 먼저 연결 끊는 경우 iocount 1 감소? -> 플래그를 도입해야 하나?

덤프파일
CrashDump의 MiniDumpWriteDump 의 MiniDumpWithFullMemory는 정확히 어떤 메모리를 찍는거임?
워킹셋 사이즈 파일명 포함 이유?
카운트는 무슨 의미? 여러파일에서 0바이트는 잘못된거임? 파일 하나만 생성해도 괜찮음?
덤프에서 메모리 NP풀 사용량? 즉 모니터링 기능? 모니터링에는 기록의 역할은 없지 않음?


Lock 없이, sendQ 어케 동기화? 인큐가 두번이라, 큐 자체 락으로는 힘들다..
```

## DevLog
### 이전
```text
문제 발생 -> 원인 완벽하게 파악 (디버깅, 로그, ...) -> 해결

지연삭제 -> 이터레이터로 인한 문제 방지. 즉 배열은 적용 x

accept thread 분리, accept()는 블로킹으로 why? 쓸데없이 도는 코드는 절대 없어야함
```
### 26-06-03

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

rpc 복습 필요
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

```

### 26-06-12
```text
Server, SessionManager 클래스 -> LanServer 클래스 전면수정


```
