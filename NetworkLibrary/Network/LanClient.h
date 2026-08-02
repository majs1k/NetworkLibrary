#pragma once
#define WIN32_LEAN_AND_MEAN
#include <string>
#include <unordered_map>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "Session.h"
#include "../Utils/RingBuffer.h"
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

	bool Connect();
	bool Disconnect();
	bool SendPacket(Packet& packet);

	virtual void OnEnterJoinServer() = 0;

	virtual void OnLeaveServer() = 0;

	virtual void OnRecv(__int64 sessionId, Packet& packet) = 0;

	virtual void OnError(int errorCode, wchar_t* str) = 0;

	static unsigned int __stdcall WorkerThread(void* param);

	// ------------------------------------------------------- //

	void RecvPost(Session* session);
	void CompleteRecv(Session* session, int numOfBytes);
	void SendPost(Session* session);
	void CompleteSend(Session* session, int numOfBytes);
	void DecrementIoCount(Session* session);
};
