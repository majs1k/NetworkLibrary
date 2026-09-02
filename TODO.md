## TODO
```text
TPS -> 서버와 더미 분리, 스레드 수 수정, 재연결 x, 조건 고정

수신 최적화 : shared_ptr과 memory pool
컨텐츠 단에서만 패킷 메모리풀 적용?
메모리풀 락 추가
Packet 크기 제한 & 예외 처리
warning 제거 , 캐스팅 수정

SPSC 큐 적용
acceptex


Player 테이블에 user_id는 index가 필요할까?

캐릭터 정보 엑셀에서 텍스트 ->서버 클라 업로드
상점 구매 시 캐릭터  랜덤 추가

캐릭터 정보창 + 정렬 + 최대한도 (클라/서버) + 판매

게임 중 공격 패킷

```

## Question
```text
worker 1 client 1 oversend 100 loop 0 -> zero copy 유리 (패킷 크기 약 1000byte)

스레드간 전달 방식 -> 구조체 vs 직렬화버퍼

vector 를 써야할상황? (데이터가 적을떄??)

```
