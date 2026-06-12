#pragma once
#include <string>
#include <winsock2.h>
#include "../Utils/RingBuffer.h"
#include "../Utils/Lock.h"

// ------------------------------------------------------- //

enum class IOType
{
	SEND = 0,
	RECV = 1,
};

struct OverlappedEx
{
	WSAOVERLAPPED overlapped;
	IOType type;
};

// ------------------------------------------------------- //

class Packet;

#define SEND_SIZE				10000
#define RECV_SIZE				10000

class Session
{
private:
	struct HEADER
	{
		unsigned short size;
	};

	SOCKET socket_;
	// 고유값. sessionList의 인덱스로 활용 x
	// 네트워크 연결에 대한 로직/로그에서 사용
	// 소켓핸들처럼 컨텐츠에서 사용 (컨텐츠 코드가 간단해짐)
	__int64 sessionId_;

	std::wstring ip_;
	int port_;

	RingBuffer sendQueue_{ SEND_SIZE };
	RingBuffer recvQueue_{ RECV_SIZE };

	OverlappedEx sendOverlapped_;
	OverlappedEx recvOverlapped_;

	LONG ioCount_;
	LONG sendPending_;

	Lock sessionLock_;

public:
	~Session();
	void initialize(SOCKET socket, std::wstring ip, int port, __int64 id);

	SOCKET socket() const;
	__int64 id() const;

	std::wstring ip() const;
	int port() const;

	void postRecv();
	void completeRecv(int numOfBytes);

	void postSend();
	void completeSend(int numOfBytes);
	void sendPacket(Packet& packet);

	void decrementIOCount();
};
