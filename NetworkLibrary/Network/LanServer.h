#pragma once
#define WIN32_LEAN_AND_MEAN
#include <string>
#include <vector>
#include <unordered_map>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include "../Utils/Singleton.h"
#include "../Utils/Lock.h"

class Session;
class Packet;

class LanServer
{
protected:
	SOCKET listenSocket_;

	std::wstring serverIp_;
	int serverPort_;

	HANDLE hIOCP_;
	std::vector<HANDLE> hWorkerThread_;
	HANDLE hAcceptThread_;
	HANDLE hMonitorThread_;
	HANDLE hExitEvent;
	int concurrentThreadCount_;
	int workerThreadCount_;

	// rehash 일어나면(갑자기 삽입 했을때 등..) 이터레이터 무효화 주의
	std::unordered_map<__int64, Session*> sessionMap_;
	LONG sessionCount_ = 0;
	int sessionMax_;
	Lock sessionMapLock_;
	__int64 idSeed_ = 0;

	LONG acceptTps_;
	LONG recvMessageTps_;
	LONG sendMessageTps_;

	LONG acceptCount_;
	LONG recvMessageCount_;
	LONG sendMessageCount_;

public:
	LanServer();
	~LanServer();

	bool start(std::wstring ip, int port, int sessionMax, int concurrentCount, int workerCount);
	void stop();
	int sessionCount() const;

	/// false가 반환되면 뭘 해야함???
	bool disconnect(__int64 sessionId);
	bool sendPacket(__int64 sessionId, Packet& packet);

	/// 순수 가상함수
	virtual bool onConnectionRequest(const std::wstring& ip, int port) = 0;

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
