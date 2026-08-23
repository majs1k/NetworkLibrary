## TODO
```text
성능측정은 더미테스트 TPS로
더미테스트 스레드를 늘리면 왜 tps 감소? -> 서버와 더미 분리

프로파일러 더미 테스트 시 재연결 금지

shared_ptr과 memory pool 같이 사용

DB 스레드 1개 db 요청큐 응답큐 1개 -> 직렬 처리
로직스레드에서만 월드 상태 수정 -> 동기화 문제 x

Packet 크기 제한 & 예외처리 적용
메세지 쿨타임 처리 (3번 허용 후 종료)

warning 제거 , 캐스팅 수정

***서버 전환시 아래 교체 필요***
SendPacket() / CompletRecv() / Packet Class Define값 / ServerMain

---------------------
더미 TPS

기본
175만
200만
195만

remote
48만
---------------------
```

## Question
```text
lock_guard 사용시 재귀락 없애는 방법?
내부 함수를 잠금 없이 분리 (가장 추천) (잠금용/비잠금용)

worker 1 client 1 oversend 100 loop 0 -> zero copy 유리 (패킷 크기 약 1000byte)

실시간 소켓 서버는 롤백
스레드간 전달 방식 -> 구조체 vs 직렬화버퍼

LanServer를 상속한 서버에 각종 컨텐츠 자료구조들이랑 PacketProc가 들어가면, 객체지향이 전혀 아니게되는데.. 다 friend?

로직스레드랑 db스레드는 라이브러리 영역?
```
