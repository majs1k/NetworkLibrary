## TODO
```text
성능측정은 더미테스트 TPS로

더미테스트 스레드를 늘리면 왜 tps 감소하는지 확인

네이글 옵션?

메모리풀 적용 (placement new = false)

패킷큐 MPSC큐 사용시 성능 향상?

DB 스레드 1개 db 요청큐 응답큐 1개 -> 직렬 처리
로직스레드에서만 월드 상태 수정 -> 동기화 문제 x

Packet 크기 제한 & 예외처리 적용

메세지 쿨타임 처리 (3번 허용 후 종료)

warning 제거 , 캐스팅 수정

Protocol Buffer 적용

***서버 전환시 아래 교체 필요***
SendPacket() / CompletRecv() / Packet Class Define값 / ServerMain
```

## Question
```text
lock_guard 사용시 재귀락 없애는 방법?
방법 1. 내부 함수를 잠금 없이 분리 (가장 추천) (잠금용/비잠금용)

shared_ptr과 memory pool 같이 사용

프로파일러 더미 테스트 시 재연결 금지

메모리풀 placement new 활용도?

타입의 처리?

worker 1 client 1 oversend 100 loop 0 -> zero copy 유리 (패킷 크기 약 1000byte)

Packet의 수명 유지가 필요? 패킷을 스레드간 전달하는 방식의 장점?
```
