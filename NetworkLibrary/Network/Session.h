#pragma once
#include <string>
#include <memory>
#include <mutex>
#include <WinSock2.h>
#include "../Utils/RingBuffer.h"

//#define SEND_SIZE				500
#define SEND_SIZE				20000
#define RECV_SIZE				20000

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

class Packet;
class RecvPacket;

/// TODO: 세션 삭제시 소켓 close
struct Session
{
	SOCKET socket_;
	// 고유값. sessionList의 인덱스로 활용 x
	// 네트워크 연결에 대한 로직/로그에서 사용
	// 소켓핸들처럼 컨텐츠에서 사용 (컨텐츠 코드가 간단해짐)
	__int64 sessionId_;

	std::wstring ip_;
	int port_;


	//TRingBuffer<Packet*, SEND_SIZE> sendQueue_;
	RingBuffer sendQueue_{ SEND_SIZE };

	//std::shared_ptr<RingBuffer> recvQueue_;
	RingBuffer recvQueue_{ RECV_SIZE };


	OverlappedEx sendOverlapped_;
	OverlappedEx recvOverlapped_;

	LONG ioCount_;
	LONG sendPending_;

	LONG sendPacketCount_ = 0;

	std::recursive_mutex sessionLock_;

	void Initialize(SOCKET socket, std::wstring ip, int port, __int64 id)
	{
		socket_ = socket;
		sessionId_ = id;

		ip_ = ip;
		port_ = port;

		sendQueue_.Clear();


		//recvQueue_ = std::make_shared<RingBuffer>(RECV_SIZE);


		sendOverlapped_.type = IOType::SEND;
		recvOverlapped_.type = IOType::RECV;

		ioCount_ = 0;
		sendPending_ = false;
	}
};
