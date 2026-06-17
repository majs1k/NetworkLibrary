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

프로파일러도 락? 워커 스레드 1개로만 프로파일링 가능?

disconnect(), sendPacket() bool false 반환시 동작??

서버쪽에서 먼저 연결 끊는 경우??

네트워크 패킷 헤더에 타입이 없으니 타입을 포함해서 길이 측정?

세션 삭제 문제는 스마트포인터 사용한다고 해결 안된다??

프로파일링 내용을 그래프로 삽입?
```
