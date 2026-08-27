## TODO
```text
성능측정은 더미테스트 TPS로
더미테스트 스레드를 늘리면 왜 tps 감소? -> 서버와 더미 분리

프로파일러 더미 테스트 시 
스레드 수 수정, 재연결 x, 조건 고정

shared_ptr과 memory pool

로직스레드에서만 월드 상태 수정

Packet 크기 제한 & 예외처리

warning 제거 , 캐스팅 수정

팀플과 지금의 구현방식차이 (jwt로 인증 등등.. 서버권위구조)


캐릭터 구매는 우선 랜덤 캐릭터로
캐릭터 delete 추가
매치 취소 기능
```

## Question
```text
worker 1 client 1 oversend 100 loop 0 -> zero copy 유리 (패킷 크기 약 1000byte)

실시간 소켓 서버는 롤백
스레드간 전달 방식 -> 구조체 vs 직렬화버퍼
```
