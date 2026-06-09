#pragma once
#define WIN32_LEAN_AND_MEAN
#include <string>
#include <WinSock2.h>
#include <Windows.h>
#include "../Utils/Singleton.h"

extern LONG cnt;


class Session;

#define SERVER_PORT			6000

#define CONCURRENT_NUM		4
#define WORKER_NUM			4

class Server : public Singleton<Server>
{
private:
	SOCKET listenSocket_;

	std::wstring ip_;
	int port_;

	HANDLE hIOCP_;

	HANDLE hWorkerThread_[WORKER_NUM];
	HANDLE hAcceptThread_;

public:
	Server();
	~Server();

	void initialize();
	void serverExit();
	void onAccept(SOCKET socket, SOCKADDR_IN sockaddr);

	static unsigned int __stdcall acceptThread(void* param);
	static unsigned int __stdcall workerThread(void* param);
};
