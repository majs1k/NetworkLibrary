#pragma once
#define WIN32_LEAN_AND_MEAN
#include <string>
#include <unordered_map>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include "../Utils/RingBuffer.h"
#include "../Utils/Lock.h"

// ------------------------------------------------------- //

struct HEADER
{
	unsigned short size;
};

// ------------------------------------------------------- //

#define PACKET_CODE				0x89

struct FIGHTER_HEADER
{
	unsigned char code;
	unsigned char size;
	//unsigned char type;
};

// ------------------------------------------------------- //

#define SEND_SIZE				10000
#define RECV_SIZE				10000

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

/// TODO: 세션 삭제시 소켓 close 해야함.
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

	OverlappedEx sendOverlapped_;
	OverlappedEx recvOverlapped_;

	LONG ioCount_;
	LONG sendPending_;

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

// ------------------------------------------------------- //

class Packet;

#define SERVER_PORT			6000

#define CONCURRENT_COUNT		4
#define WORKER_COUNT			4

class LanClient
{
private:
	SOCKET clientSocket_;

	std::wstring serverIp_;
	int serverPort_;

	HANDLE hIOCP_;
	HANDLE hWorkerThread_[WORKER_COUNT];

	// 변수명?
	Session* clientSession_;

public:
	LanClient();
	~LanClient();

	bool connect();
	bool disconnect();
	bool sendPacket(Packet& packet);

	virtual void onEnterJoinServer() = 0;

	virtual void onLeaveServer() = 0;

	virtual void onRecv(__int64 sessionId, Packet& packet) = 0;

	virtual void onError(int errorCode, wchar_t* str) = 0;

	static unsigned int __stdcall workerThread(void* param);

	// ------------------------------------------------------- //

	void postRecv(Session* session);
	void completeRecv(Session* session, int numOfBytes);

	void postSend(Session* session);
	void completeSend(Session* session, int numOfBytes);

	void decrementIoCount(Session* session);
};
