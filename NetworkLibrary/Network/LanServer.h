#pragma once
#define WIN32_LEAN_AND_MEAN
#include <string>
#include <unordered_map>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include "../Utils/Singleton.h"
#include "../Utils/Lock.h"

class Session;
class Packet;

#define MAX_SESSION			200

#define SERVER_PORT			6000

#define CONCURRENT_NUM		4
#define WORKER_NUM			4

class LanServer
{
private:
	SOCKET listenSocket_;

	std::wstring ip_;
	int port_;

	HANDLE hIOCP_;
	HANDLE hWorkerThread_[WORKER_NUM];
	HANDLE hAcceptThread_;
	HANDLE hMonitorThread_;
	HANDLE hExitEvent;

	// rehash 일어나면(갑자기 삽입 했을때 등..) 이터레이터 무효화 주의
	std::unordered_map<__int64, Session*> sessionMap_;
	LONG sessionCount_;
	Lock sessionMapLock_;
	__int64 idSeed_;

	LONG acceptTps_;
	LONG recvMessageTps_;
	LONG sendMessageTps_;

	LONG acceptCount_;
	LONG recvMessageCount_;
	LONG sendMessageCount_;

public:
	LanServer();
	~LanServer();

	void start();
	void stop();
	int sessionCount() const;

	/// false가 반환되면 뭘 해야함???
	bool disconnect(__int64 sessionId);
	bool sendPacket(__int64 sessionId, Packet& packet);

	/// 순수 가상함수
	virtual bool onConnectionRequest(const std::wstring& ip, int port);

	// 인자 미정
	virtual void onAccept(__int64 sessionId) = 0;

	virtual void onRelease(__int64 sessionId) = 0;

	virtual void onRecv(__int64 sessionId, Packet& packet) = 0;

	virtual void onError(int errorCode, wchar_t* str) = 0;

	int acceptTps();
	int recvMessageTps();
	int sendMessageTps();
	void printTps();

	static unsigned int __stdcall acceptThread(void* param);
	static unsigned int __stdcall workerThread(void* param);
	static unsigned int __stdcall mornitorThread(void* param);

	Session* findSession(__int64 sessionId);
	void increaseRecvMessageTps();
};

extern LanServer* server;
