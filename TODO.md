## TODO
```text
DB 연동
메세지 쿨타임 처리 (3번 허용 후 종료)
c# -> NetworkStream + rpc 코드
warning 제거 , 캐스팅 수정

zero copy 프로파일링 테스트 + 패턴 파악
로컬 리모트 테스트 등~
```

## Question
```text
예외처리 try-catch?

프로파일러도 락? 워커 스레드 1개로만 프로파일링 가능?

메모리 풀 placement new 활용도??
메모리 릭(custom new-delete)/오버플로우(no access page) 라이브러리 활용도?

LanServer 서버쪽에서 먼저 연결 끊을떈? disconnect(), sendPacket() bool false 반환시 동작??

네트워크 패킷 헤더에 타입이 없으니 타입을 포함해서 길이 측정?

세션 삭제 문제는 스마트포인터 사용한다고 해결 안된다??
```
