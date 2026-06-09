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
포트폴리오 최종 목표는 마을 단위 컨텐츠 구현
```

## Question
```text
패킷 핸들러에서 컨텐츠 수정 -> friend? setter?
예외처리 try-catch?

라이브러리의 헤더파일에 define 값을 맘대로 바꿀수 있나?

메모리 풀 관련
메모리 릭(custom new-delete)/오버플로우(no access page) 라이브러리 활용도?
placement new 활용도??

로그에서 락 걸고 큐에 담은후 1초에 한번씩 flush 하는 경우 - 문제 발생 x?

RPCProxy -> 인자가 sessionId? sendPacket에선 id로?

서버쪽에서 먼저 연결 끊는 경우 iocount 1 감소? -> 결국은 플래그를 도입해야 하나?
```

## DevLog
### 이전
```text
지연삭제 -> 이터레이터로 인한 문제 방지. 즉 배열은 적용 x

accept thread 분리, accept()는 블로킹으로 why? 쓸데없이 도는 코드는 없어야하므로..
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
sessionId는 세션마다 고유한 값이므로 문제 x
```

### 26-06-06
```text
폴더명 변경시 vcxproj 파일이 업데이트가 안되어서 문제 발생 가능 -> 새 프로젝트 만들어서 옮기기
~~select / iocp 라이브러리 통합 및 폴더 트리 추가~~

세션 종료시 postSend()에서 접근 못하게 ioCount 확인하는 코드 추가

rpc 복습 필요

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
GQCS 반환시 numOfBytes 체크 필요 x

windows.h -> 반드시 winsock 밑에!! 헤더 include 하면서 순서 꼬일수 있음 주의
#define WIN32_LEAN_AND_MEAN 추가도 가능 but windows.h의 헤더들이 많이 날라감..

```

### 26-06-08
```text
select 모델 삭제 (레포지토리 분리)

WSARecv()시 등록과 완료시 ioCount 변화 적용
ref count는 한 작업이 종료될때까지 들고 있다가 완료되면 감소. 0이 되면 삭제.
postSend()는 WSASend() 호출 전에 iocount 늘려야
다른 스레드에서 send 완료통지로 ioCount 내려서 0이 되는 상황 방지

sendPacket()에서 ioCount 0인지 확인하고 진입하는 코드 삭제
```

### 26-06-09
```text
Server 클래스의 소멸자 로직의 로그 출력이 안되는 상황 발생 (싱글톤으로 인한 소멸 순서 미보장)
->selectserver의 소멸자 로직을 별도의 종료함수로 분리

더미테스트
클라 1 / 오버센드 1 / 연결종료 x / 딜레이 0 / TPS 약 20000
어느 시점 갑자기 TPS 0으로 떨어짐. 서버에선 세션 삭제 문구 뜨지 않음
워커스레드 종료는 없음 확인

와이어샤크
소켓 삭제 안되었음
더미에서 송신 자체가 중단됨?? (이건 더미를 리모트로 해서 소켓 삭제되는지 확인될듯)

전역변수 만들고 스택으로 접근해보니 ioCount가 1이라는 상황 확인하였음!!

```
