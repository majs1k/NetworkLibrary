#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

#include <process.h>
#include "LanServer.h"
#include "Session.h"
#include "../Utils/Packet.h"
#include "../Utils/Logger.h"
#include "../Utils/Profiler.h"
#include <Windows.h>

LanServer::LanServer()
{
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		ERR(L"WSAStartup() error");
}

LanServer::~LanServer()
{
	WSACleanup();
}

bool LanServer::Start(std::wstring ip, int port, int sessionMax, int concurrentCount, int workerCount)
{
	serverIp_ = ip;
	serverPort_ = port;
	sessionMax_ = sessionMax;
	sessionMap_.reserve(sessionMax_);
	concurrentThreadCount_ = concurrentCount;
	workerThreadCount_ = workerCount;

	listenSocket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket_ == INVALID_SOCKET)
		ERR(L"socket() error");

	// 링거 옵션 - closesocekt() 호출시 즉시 리턴, 연결 강제 종료
	// 서버 클라 둘다 설정할 것
	LINGER lin;
	lin.l_onoff = 1;
	lin.l_linger = 0;
	setsockopt(listenSocket_, SOL_SOCKET, SO_LINGER, (char*)&lin, sizeof(lin));

	// 네이글 옵션 해제
	int nagleFlag = 1;
	setsockopt(listenSocket_, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&nagleFlag), sizeof(nagleFlag));

	// L4 송신버퍼 사이즈 옵션
	int sndBufSize = 0;
	//setsockopt(listenSocket_, SOL_SOCKET, SO_SNDBUF, (const char*)(&sndBufSize), sizeof(sndBufSize));

	int size = 0;
	int len = sizeof(size);
	getsockopt(listenSocket_, SOL_SOCKET, SO_SNDBUF, (char*)(&size), &len);

	std::cout << "SO_SNDBUF = " << size << std::endl;

	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	InetPton(AF_INET, serverIp_.c_str(), &serverAddr.sin_addr);
	serverAddr.sin_port = htons(serverPort_);

	if (bind(listenSocket_, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		ERR(L"bind() error");

	if (listen(listenSocket_, SOMAXCONN) == SOCKET_ERROR)
		ERR(L"listen() error");

	hIOCP_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, concurrentThreadCount_);

	for (int i = 0; i < workerThreadCount_; i++)
	{
		HANDLE hThread = (HANDLE)_beginthreadex(nullptr, 0, WorkerThread, this, 0, nullptr);
		hWorkerThread_.push_back(hThread);
	}

	hAcceptThread_ = (HANDLE)_beginthreadex(nullptr, 0, AcceptThread, this, 0, nullptr);

	hMonitorThread_ = (HANDLE)_beginthreadex(nullptr, 0, MornitorThread, this, 0, nullptr);

	// manual reset
	hExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	LOG_INFO(L"[NETWORK] server start");

	return true;
}

void LanServer::Stop()
{
	// accept thread 종료
	closesocket(listenSocket_);
	WaitForSingleObject(hAcceptThread_, INFINITE);
	CloseHandle(hAcceptThread_);

	// worker thread 종료
	for (int i = 0; i < workerThreadCount_; i++)
	{
		PostQueuedCompletionStatus(hIOCP_, 0, 0, nullptr);
	}

	WaitForMultipleObjects(workerThreadCount_, hWorkerThread_.data(), TRUE, INFINITE);

	for (int i = 0; i < workerThreadCount_; i++)
	{
		CloseHandle(hWorkerThread_[i]);
	}

	// monitor thread 종료
	SetEvent(hExitEvent);
	WaitForSingleObject(hMonitorThread_, INFINITE);
	CloseHandle(hMonitorThread_);
	CloseHandle(hExitEvent);

	// 세션맵 정리
	for (auto& session : sessionMap_)
	{
		delete session.second;
	}

	//sessionMap_.clear();

	// IOCP 삭제
	CloseHandle(hIOCP_);

	LOG_INFO(L"[NETWORK] server exit");
}

int LanServer::SessionCount() const
{
	return sessionCount_;
}

int LanServer::SessionMax() const
{
	return sessionMax_;
}

bool LanServer::Disconnect(__int64 sessionId)
{
	sessionMapLock_.lock();

	auto it = sessionMap_.find(sessionId);

	if (it == sessionMap_.end())
	{
		sessionMapLock_.unlock();

		return false;
	}

	Session* session = (*it).second;

	sessionMapLock_.unlock();

	closesocket(session->socket_);

	return true;
}

/// TestServer
bool LanServer::SendPacket(__int64 sessionId, Packet* packet)
{
	sessionMapLock_.lock();

	auto it = sessionMap_.find(sessionId);

	if (it == sessionMap_.end())
	{
		sessionMapLock_.unlock();

		return false;
	}

	Session* session = (*it).second;

	session->sessionLock_.lock();

	//if (ioCount_ == 0)
	//	return;

	//if(InterlockedIncrement(&ioCount_) == 1)
	//	return;


	TEST_HEADER header;
	header.size_ = packet->UseSize();

	//session->sendQueue_.Lock();

	session->sendQueue_.Enqueue((char*)&header, sizeof(TEST_HEADER));
	session->sendQueue_.Enqueue(packet->GetBufferPtr(), packet->UseSize());

	delete packet;

	//session->sendQueue_.Unlock();


	/// FighterServer
	//FIGHTER_HEADER header;
	//header.code = PACKET_CODE;
	//header.size = packet->UseSize() - sizeof(unsigned char);

	//session->sendQueue_.Lock();

	//session->sendQueue_.Enqueue((char*)&header, sizeof(FIGHTER_HEADER));
	//session->sendQueue_.Enqueue(packet->GetBufferPtr(), packet->UseSize());

	//delete packet;
	//
	//session->sendQueue_.Unlock();


	this->SendPost(session);

	sessionMapLock_.unlock();

	session->sessionLock_.unlock();

	InterlockedIncrement(&sendMessageCount_);

	return true;
}

bool LanServer::SendPacketZeroCopy(__int64 sessionId, Packet* packet)
{
	//PRO(L"sendPacket() 2");

	sessionMapLock_.lock();

	auto it = sessionMap_.find(sessionId);

	if (it == sessionMap_.end())
	{
		sessionMapLock_.unlock();

		return false;
	}

	Session* session = (*it).second;

	std::lock_guard<std::recursive_mutex> lock(session->sessionLock_);

	sessionMapLock_.unlock();

	//session->sendQueueT_.Lock();

	// 패킷 헤더 설정
	Packet* header = new Packet();
	*header << packet->UseSize();

	session->sendQueue2_.Enqueue(header);
	session->sendQueue2_.Enqueue(packet);

	//session->sendQueueT_.Unlock();

	this->SendPostZeroCopy(session);

	InterlockedIncrement(&sendMessageCount_);

	return true;
}

unsigned int __stdcall LanServer::AcceptThread(void* param)
{
	LanServer* server = (LanServer*)param;

	while (1)
	{
		SOCKADDR_IN clientAddr;
		int addrLen;

		addrLen = sizeof(clientAddr);
		SOCKET clientSock = accept(server->listenSocket_, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSock == INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			if (error == WSAENOTSOCK || error == WSAEINTR)
			{
				LOG_INFO(L"[NETWORK] accept thread exit");

				return 0;
			}
			else
			{
				ERR(L"accept() error");
			}
		}

		InterlockedIncrement(&server->acceptCount_);

		WCHAR str[16];
		InetNtop(AF_INET, &clientAddr.sin_addr, str, 16);
		std::wstring ip = str;
		int port = ntohs(clientAddr.sin_port);

		if (!server->OnConnectionRequest(ip, port))
		{
			closesocket(clientSock);
			continue;
		}

		Session* session = new Session();
		// idSeed_는 이 스레드에서만 변경 가능하므로 인터락 적용 x
		session->Initialize(clientSock, ip, port, ++(server->idSeed_));

		server->sessionMapLock_.lock();

		server->sessionMap_.insert({ session->sessionId_, session });

		server->sessionMapLock_.unlock();

		InterlockedIncrement(&(server->sessionCount_));

		//LOG_INFO(L"[NETWORK] session create ip=%s port=%d count=%d", ip.c_str(), port, server->sessionCount_);

		CreateIoCompletionPort((HANDLE)clientSock, server->hIOCP_, (ULONG_PTR)session, 0);

		server->RecvPost(session);

		// 위치 주의
		server->OnAccept(session->sessionId_);
	}

	return 0;
}

unsigned int __stdcall LanServer::WorkerThread(void* param)
{
	LanServer* server = (LanServer*)param;

	DWORD numOfBytes;
	Session* session;
	OVERLAPPED* overlapped;

	while (1)
	{
		//session = nullptr;
		//numOfBytes = 0;

		// GQCS 호출 반환시 overlapped 구조체 무조건 세팅됨
		BOOL ret = GetQueuedCompletionStatus(server->hIOCP_, &numOfBytes, (PULONG_PTR)&session, &overlapped, INFINITE);

		if (overlapped == nullptr && session == nullptr && numOfBytes == 0)
		{
			LOG_INFO(L"[NETWORK] worker thread exit");

			return 0;
		}

		if (!ret)
		{
			int error = GetLastError();

			// 실패한 IO (연결 끊김)
			// 완료 패킷을 꺼내서, 실패한 IO에 대한 정보를 매개변수에 저장함
			if (overlapped != nullptr)
			{
				// 64 상대가 연결을 끊었을때 (numOfBytes == 0)
				// 1236 clientSock 닫았을때
				if (error == ERROR_NETNAME_DELETED || error == ERROR_CONNECTION_ABORTED)
				{
					server->DecrementIoCount(session);

					continue;
				}
				// 121 트래픽 많으면 네트워크 연결 끊어버림
				// 리모트환경 테스트시 발생
				// 트래픽 줄여서 다시 테스트
				else if (error == ERROR_SEM_TIMEOUT)
				{
					__debugbreak();
				}

				LOG_INFO(L"GQCS() error: %d", error);

				__debugbreak();

				continue;
			}
			// 완료 패킷을 큐에서 제거 안함, 매개변수 값 의미 x
			else
			{
				if (error == WAIT_TIMEOUT)
				{
					continue;
				}
				// 735 CP 삭제시
				else if (error == ERROR_ABANDONED_WAIT_0)
				{
					// 스레드 종료
					return 0;
				}
				else
				{
					// GQCS 잘못 호출
					// 이 상황 배제
					return 0;
				}
			}
		}

		if (((OverlappedEx*)overlapped)->type == IOType::RECV)
		{
			// 상대가 closesocket()시 (rst x) 발생
			if (numOfBytes == 0)
			{
				server->DecrementIoCount(session);
			}
			else
			{
				server->CompleteRecv(session, numOfBytes);
			}
		}
		else
		{
			server->CompleteSend(session, numOfBytes);
			//server->completeSendSkipCopy(session, numOfBytes);
		}
	}

	return 0;
}

unsigned int __stdcall LanServer::MornitorThread(void* param)
{
	LanServer* server = (LanServer*)param;

	// 시간차는 int형으로 선언
	int sleepTime = 0;
	DWORD lastTime = timeGetTime();

	system("cls");

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// 콘솔창 커서 숨기기
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);

	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &cursorInfo);

	while (1)
	{
		DWORD dw = WaitForSingleObject(server->hExitEvent, sleepTime);

		switch (dw)
		{
		case WAIT_TIMEOUT:
		{
			DWORD curTime = timeGetTime();

			sleepTime = CLOCKS_PER_SEC - (curTime - lastTime);
			lastTime += CLOCKS_PER_SEC;

			LONG acceptTps = InterlockedExchange(&server->acceptCount_, 0);
			LONG recvMessageTps = InterlockedExchange(&server->recvMessageCount_, 0);
			LONG sendMessageTps = InterlockedExchange(&server->sendMessageCount_, 0);

			COORD pos = { 0, 0 };
			SetConsoleCursorPosition(hConsole, pos);

			std::cout << "Acpt TPS : " << acceptTps << std::endl;
			std::cout << "Recv TPS : " << recvMessageTps << std::endl;
			std::cout << "Send TPS : " << sendMessageTps << std::endl;
		}
		break;

		case WAIT_OBJECT_0:
			LOG_INFO(L"[NETWORK] monitor thread exit");
			return 0;
		}
	}

	return 0;
}
