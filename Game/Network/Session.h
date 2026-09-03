#pragma once
#include <string>
#include <memory>
#include <mutex>
#include <WinSock2.h>
#include "../Utils/RingBuffer.h"

// TOOD: 버퍼 사이즈 조정
//#define SEND_SIZE				500
#define SEND_SIZE				20000
#define RECV_SIZE				20000

enum class IO_TYPE
{
	SEND = 0,
	RECV = 1,
};

struct OverlappedEx
{
	WSAOVERLAPPED overlapped;
	IO_TYPE type;
};

class Packet;

// TODO: 세션 삭제시 소켓 close
struct Session
{
	SOCKET socket_;
	// 고유값. sessionList의 인덱스로 활용 x
	// 컨텐츠 코드에서 sessionId로만 접근 (컨텐츠 구현 용이)
	__int64 sessionId_;

	std::string ip_;
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

	void Initialize(SOCKET socket, std::string ip, int port, __int64 id)
	{
		socket_ = socket;
		sessionId_ = id;

		ip_ = ip;
		port_ = port;

		sendQueue_.Clear();


		//recvQueue_ = std::make_shared<RingBuffer>(RECV_SIZE);


		sendOverlapped_.type = IO_TYPE::SEND;
		recvOverlapped_.type = IO_TYPE::RECV;

		// 초기화시 참조 카운트를 1로 시작
		ioCount_ = 1;
		sendPending_ = false;
	}
};
