## TODO
```text

성능측정은 TPS로

메모리풀 적용 (placement new = false로 할 것)
Protocol Buffer 적용

DB 커넥션 & 스레드 1개로 설계
로직스레드의 IO큐와 DB큐 분리 -> why? 로직스레드에서만 플레이어, 월드 상태 수정 -> 동기화 편리
메모리에서 직렬 처리 보장하니 필요시 트랜잭션 사용할것

Packet 크기 제한 & 예외처리 적용

메세지 쿨타임 처리 (3번 허용 후 종료)

warning 제거 , 캐스팅 수정

```

## Question
```text
네이글 옵션?

DLL을 만드는 방법? 클래스로는 불가?

예외처리 try-catch?

프로파일러 더미 테스트 시 재연결 금지

메모리풀 placement new 활용도?

disconnect(), sendPacket() false 반환시 동작??

타입은 헤더가 아닌 Packet(직렬화 버퍼)에 포함?

sendQ recvQ 수정

worker 1 client 1 oversend 100 loop 0 -> zero copy 유리 (패킷 크기 약 1000byte)

기능별 서버 분산시 게임서버와 웹서버 분리?? 클라이언트는 각각 연결? db는 각각 따로 존재?

Packet을 로직스레드로 전달할때, sessionId도 전달 (메세지 큐)

SPSC큐 를 잡큐로 사용시 성능 향상을 확인

맵별 락?

레이턴시?
```
