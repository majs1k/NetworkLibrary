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

네트워크 패킷 헤더에 타입이 없다??
RPC 적용된 경우에는 
Proxy 코드에서 헤더를 Packet 구조체에 넣고 sendPacket 진입?
onRecv 에서 타입을 Packet 구조체에서 뺀 후 packetProc로 타입 전달?
RPC 코드들은 인자가 Session* 대신에 sessionId?

서버쪽에서 먼저 연결 끊는 경우 (컨텐츠 종료 등)
"컨텐츠" 측에서 세션 찾아서 iocount 감소? 플래그 도입?

server 전역변수 하나만 존재? 메인함수에서 선언?
만약 아니면 session 클래스에서 어떻게 사용할지

onAccept(), onEnterJoinServer() 은 recv 등록하기 전에 호출?

로그 파일 출력시 utf-8??? 문자열도 utf-8 세트?
```
