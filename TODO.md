## TODO
```text
DB 연동
메세지 쿨타임 처리 (3번 허용 후 종료)
c# -> NetworkStream + rpc 코드
라이브러리 64비트 호환
warning 제거 , 캐스팅 수정

zero copy 프로파일링 테스트
클라 1개 아니면 프로파일러 문제 발생?
로컬 리모트 테스트 등~

+ 패턴 파악
```

## Question
```text
예외처리 try-catch?

라이브러리의 헤더파일에 define값을 맘대로 바꿀수 있나?

메모리 풀 관련
메모리 릭(custom new-delete)/오버플로우(no access page) 라이브러리 활용도?
placement new 활용도??

로그에서 락 걸고 큐에 담은후 1초에 한번씩 flush 하는 경우 - 문제 발생 x?

세션락과 상관없이 sendQ가 사용되는 모든 부분에 락 적용은 고정임?
프로파일러도 락? 워커 스레드 1개로만 프로파일링 가능?

LanServer를 상속받은 클래스는 컨텐츠 코드?
LanServer의 메서드들은 전부 public? 멤버변수들은 private? protected? (sessionCount)
disconnect(), sendPacket() bool false 반환시 동작??

서버쪽에서 먼저 연결 끊는 경우 (컨텐츠 종료 등)
"컨텐츠"에서 어케함? (server 인스턴스가 애초에 없음)

onAccept(), onEnterJoinServer() 은 recv 등록하기 전에 호출?

네트워크 패킷 헤더에 타입이 없다??
RPC 적용된 경우에는 
Proxy 코드에서 헤더를 Packet 구조체에 넣고 sendPacket 진입?
onRecv 에서 타입을 Packet 구조체에서 뺀 후 packetProc로 타입 전달?
RPC 코드들은 인자가 Session* 대신에 sessionId?

rpc 자동생성 코드 Server Client 별개?

로직 스레드는 상속받은 server 클래스에서 구현하고, 별도의 start() 함수 호출?

세션 삭제 문제는 스마트포인터 사용한다고 해결 안된다??

메모리풀 방식으로 바뀌었을떄 락 거는 개념이 바뀌는게 있음?

프로파일링 내용을 그래프로 삽입?
```
