#pragma once
#define WIN32_LEAN_AND_MEAN
#include <string>
#include <unordered_map>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "Session.h"
#include "../Utils/RingBuffer.h"
#include "../Utils/Lock.h"
#include <Windows.h>

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

	// º¯¼ö¸í?
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

	void recvPost(Session* session);
	void completeRecv(Session* session, int numOfBytes);
	void sendPost(Session* session);
	void completeSend(Session* session, int numOfBytes);
	void decrementIoCount(Session* session);
};
