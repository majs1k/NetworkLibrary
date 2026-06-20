#pragma once
#include <string>
#include <WinSock2.h>
#include "../Utils/RingBuffer.h"
#include "../Utils/RingBufferT.h"
#include "../Utils/Lock.h"
#include <Windows.h>

#define SEND_SIZE				10000
#define RECV_SIZE				10000

#define SEND_CNT				2000

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
class SendPacket;

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

	RingBuffer sendQueue_{ SEND_SIZE };
	RingBuffer recvQueue_{ RECV_SIZE };

	/// 세션 종료시 정리 필요!!! (count 등...)
	BufferT<SendPacket*, SEND_CNT> sendQueueT_;

	OverlappedEx sendOverlapped_;
	OverlappedEx recvOverlapped_;

	LONG ioCount_;
	LONG sendPending_;

	LONG sendPacketCount_ = 0;

	Lock sessionLock_;

	void initialize(SOCKET socket, std::wstring ip, int port, __int64 id)
	{
		socket_ = socket;
		sessionId_ = id;

		ip_ = ip;
		port_ = port;

		sendQueue_.clear();
		recvQueue_.clear();

		sendOverlapped_.type = IOType::SEND;
		recvOverlapped_.type = IOType::RECV;

		ioCount_ = 0;
		sendPending_ = false;
	}
};
