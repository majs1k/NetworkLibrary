#pragma comment(lib, "ws2_32.lib")
#include <process.h>
#include "Server.h"	
#include "Session.h"
#include "../Utils/Packet.h"
#include "../Utils/Logger.h"
#include "../Utils/Profiler.h"
#pragma comment(lib, "winmm.lib")

Server::Server()
	:idSeed_(0), sessionCount_(0)
{
	sessionMap_.reserve(MAX_SESSION);

	// manual reset
	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		ERR(L"WSAStartup() error");
}

Server::~Server()
{
	WSACleanup();
}

void Server::start()
{
	//ip_ = ConfigManager::getInstance().network_.ip;
	//port_ = ConfigManager::getInstance().network_.port;

	ip_ = L"0.0.0.0";
	port_ = SERVER_PORT;

	hIOCP_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, CONCURRENT_NUM);

	for (int i = 0; i < WORKER_NUM; i++)
	{
		hWorkerThread_[i] = (HANDLE)_beginthreadex(nullptr, 0, workerThread, this, 0, nullptr);
	}

	listenSocket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket_ == INVALID_SOCKET)
		ERR(L"socket() error");

	// 링거 옵션 - closesocekt() 호출시 즉시 리턴, 연결 강제 종료
	// 서버 클라 둘다 설정할 것
	LINGER optval;
	optval.l_onoff = 1;
	optval.l_linger = 0;

	if (setsockopt(listenSocket_, SOL_SOCKET, SO_LINGER, (char*)&optval, sizeof(optval)) == SOCKET_ERROR)
		ERR(L"setsockopt() error");

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
	InetPton(AF_INET, ip_.c_str(), &serverAddr.sin_addr);
	serverAddr.sin_port = htons(port_);

	if (::bind(listenSocket_, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		ERR(L"bind() error");

	if (::listen(listenSocket_, SOMAXCONN) == SOCKET_ERROR)
		ERR(L"listen() error");

	hAcceptThread_ = (HANDLE)_beginthreadex(nullptr, 0, acceptThread, this, 0, nullptr);

	hMonitorThread_ = (HANDLE)_beginthreadex(nullptr, 0, mornitorThread, this, 0, nullptr);

	LOG_INFO(L"[NETWORK] server start");
}

void Server::stop()
{
	closesocket(listenSocket_);

	for (int i = 0; i < WORKER_NUM; i++)
	{
		PostQueuedCompletionStatus(hIOCP_, 0, 0, nullptr);
	}

	WaitForSingleObject(hAcceptThread_, INFINITE);
	CloseHandle(hAcceptThread_);

	WaitForMultipleObjects(WORKER_NUM, hWorkerThread_, TRUE, INFINITE);

	for (int i = 0; i < WORKER_NUM; i++)
	{
		CloseHandle(hWorkerThread_[i]);
	}

	SetEvent(hEvent);
	WaitForSingleObject(hMonitorThread_, INFINITE);
	CloseHandle(hMonitorThread_);
	CloseHandle(hEvent);

	for (auto& session : sessionMap_)
	{
		delete session.second;
	}

	CloseHandle(hIOCP_);

	LOG_INFO(L"[NETWORK] server exit");
}

int Server::sessionCount() const
{
	return sessionCount_;
}

bool Server::disconnect(__int64 sessionId)
{
	Session* session = this->findSession(sessionId);

	if (session == nullptr)
		return false;

	mapLock_.lock();

	sessionMap_.erase(sessionId);

	mapLock_.unlock();

	delete session;

	//LOG_INFO(L"[NETWORK] session delete count=%d", sessionSize_);

	InterlockedDecrement(&sessionCount_);

	return true;
}

bool Server::sendPacket(__int64 sessionId, Packet& packet)
{
	Session* session = this->findSession(sessionId);

	if (session)
	{
		session->sendPacket(packet);

		return true;
	}
	else
		return false;
}

bool Server::onConnectionRequest(const std::wstring& ip, int port)
{
	if (sessionCount_ >= MAX_SESSION)
	{
		LOG(L"[Network] session limit over");

		return false;
	}

	{
		// TODO: lan 외부 ip / 해외 ip / DDos 공격 차단
	}

	return true;
}

void Server::onAccept(__int64 sessionId)
{

}

void Server::onRelease(__int64 sessionId)
{

}

void Server::onRecv(__int64 sessionId, Packet& packet)
{
	MESSAGE message;
	packet >> message.data_;

	Packet packet2;
	packet2 << message.data_;

	// 호출부 안에서 패킷 헤더를 삽입
	this->sendPacket(sessionId, packet2);
}

void Server::onError(int errorCode, wchar_t* str)
{

}

int Server::acceptTps()
{
	return acceptTps_;
}

int Server::recvMessageTps()
{
	return recvMessageTps_;
}

int Server::sendMessageTps()
{
	return sendMessageTps_;
}

void Server::printTps()
{
	wprintf(L"acceptTps: %d\nrecvMessageTps: %d\nsendMessageTps: %d\n", acceptTps_, recvMessageTps_, sendMessageTps_);
}

unsigned int __stdcall Server::acceptThread(void* param)
{
	Server* server = (Server*)param;

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

		/// TODO: ip.c_str() 로 바꾸면 안됨?
		wchar_t str[16];
		InetNtop(AF_INET, &clientAddr.sin_addr, str, 16);
		std::wstring ip = str;
		int port = ntohs(clientAddr.sin_port);

		if (!server->onConnectionRequest(ip, port))
			continue;

		Session* session = new Session();
		// idSeed_는 이 스레드에서만 변경 가능하므로 인터락 적용 x
		session->initialize(clientSock, ip, port, ++(server->idSeed_));

		//LOG_INFO(L"[NETWORK] session ip=%s port=%d", ip, port);

		server->mapLock_.lock();

		server->sessionMap_.insert({ session->id(), session });

		server->mapLock_.unlock();

		InterlockedIncrement(&(server->sessionCount_));

		CreateIoCompletionPort((HANDLE)clientSock, server->hIOCP_, (ULONG_PTR)session, 0);

		//LOG_INFO(L"[NETWORK] session create count=%d", server->sessionCount_);

		session->postRecv();

		server->onAccept(session->id());
	}

	return 0;
}

unsigned int __stdcall Server::workerThread(void* param)
{
	Server* server = (Server*)param;

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
				if (error == ERROR_NETNAME_DELETED)
				{
					session->decrementIOCount();

					continue;
				}
				// 1236 clientSock 닫혓을때
				// ioCount 도입했기 때문에 뜨면 안됨
				else if (error == ERROR_CONNECTION_ABORTED)
				{
					LOG_INFO(L"GQCS() error: ERROR_CONNECTION_ABORTED");

					session->decrementIOCount();

					continue;
				}

				LOG_INFO(L"GQCS() error: %d", error);

				session->decrementIOCount();

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
				session->decrementIOCount();

				continue;
			}

			session->completeRecv(numOfBytes);

			InterlockedIncrement(&server->recvMessageCount_);
		}
		else
		{
			session->completeSend(numOfBytes);

			InterlockedIncrement(&server->sendMessageCount_);
		}
	}

	return 0;
}

unsigned int __stdcall Server::mornitorThread(void* param)
{
	Server* server = (Server*)param;

	// 시간차는 int형으로 선언
	int sleepTime = 0;
	DWORD lastSecond = timeGetTime();

	while (1)
	{
		DWORD dw = WaitForSingleObject(server->hEvent, sleepTime);

		switch (dw)
		{
		case WAIT_TIMEOUT:
		{
			DWORD curTime = timeGetTime();

			while (1)
			{
				int deltaTime = curTime - lastSecond;

				sleepTime = CLOCKS_PER_SEC - deltaTime;

				lastSecond += CLOCKS_PER_SEC;

				server->acceptTps_ = InterlockedExchange(&server->acceptCount_, 0);
				server->recvMessageTps_ = InterlockedExchange(&server->recvMessageCount_, 0);
				server->sendMessageTps_ = InterlockedExchange(&server->sendMessageCount_, 0);

				if (sleepTime > 0)
					break;
			}
		}
		break;

		case WAIT_OBJECT_0:
			LOG_INFO(L"[NETWORK] monitor thread exit");
			return 0;
		}
	}
}

Session* Server::findSession(__int64 sessionId)
{
	mapLock_.lock();

	auto it = sessionMap_.find(sessionId);
	auto end = sessionMap_.end();

	mapLock_.unlock();

	if (it != end)
	{
		return (*it).second;
	}
	else
		return nullptr;
}
