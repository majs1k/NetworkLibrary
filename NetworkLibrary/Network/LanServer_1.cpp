#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

#include <WS2tcpip.h>
#include <mstcpip.h>
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
		wprintf(L"WSAStartup() error");
}

LanServer::~LanServer()
{
	WSACleanup();
}

bool LanServer::Start(std::string ip, int port, int sessionMax, int concurrentCount, int workerCount)
{
	serverIp_ = ip;
	serverPort_ = port;
	sessionMax_ = sessionMax;
	sessionMap_.reserve(sessionMax_);
	concurrentCount_ = concurrentCount;
	workerCount_ = workerCount;

	listenSocket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket_ == INVALID_SOCKET)
		wprintf(L"socket() error");

	// 링거 옵션 - closesocekt() 호출시 즉시 리턴, 연결 강제 종료
	// 서버 클라 둘다 설정할 것
	LINGER lin;
	lin.l_onoff = 1;
	lin.l_linger = 0;
	setsockopt(listenSocket_, SOL_SOCKET, SO_LINGER, (char*)&lin, sizeof(lin));

	// 네이글 off
	int nagleFlag = 1;
	setsockopt(listenSocket_, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&nagleFlag), sizeof(nagleFlag));


	//BOOL keepAlive = TRUE;

	//// TCP Keep-Alive 활성화
	//setsockopt(listenSocket_, SOL_SOCKET, SO_KEEPALIVE, reinterpret_cast<const char*>(&keepAlive), sizeof(keepAlive));

	//// Keep-Alive 설정
	//tcp_keepalive keepAliveVals{};
	//keepAliveVals.onoff = 1;
	//keepAliveVals.keepalivetime = 5000;   // 5초
	//keepAliveVals.keepaliveinterval = 1000; // 응답 없을 때 1초 간격

	//DWORD bytesReturned = 0;

	//WSAIoctl(listenSocket_, SIO_KEEPALIVE_VALS, &keepAliveVals, sizeof(keepAliveVals), nullptr, 0, &bytesReturned, nullptr, nullptr);


	// L4 송신버퍼 사이즈 옵션
	int sndBufSize = 0;
	//setsockopt(listenSocket_, SOL_SOCKET, SO_SNDBUF, (const char*)(&sndBufSize), sizeof(sndBufSize));


	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	InetPtonA(AF_INET, serverIp_.c_str(), &serverAddr.sin_addr);
	serverAddr.sin_port = htons(serverPort_);

	if (bind(listenSocket_, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		wprintf(L"bind() error");

	if (listen(listenSocket_, SOMAXCONN) == SOCKET_ERROR)
		wprintf(L"listen() error");

	hIOCP_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, concurrentCount_);

	for (int i = 0; i < workerCount_; i++)
	{
		HANDLE hThread = (HANDLE)_beginthreadex(nullptr, 0, WorkerThread, this, 0, nullptr);
		hWorkerThread_.push_back(hThread);
	}

	hAcceptThread_ = (HANDLE)_beginthreadex(nullptr, 0, AcceptThread, this, 0, nullptr);

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
	for (int i = 0; i < workerCount_; i++)
	{
		PostQueuedCompletionStatus(hIOCP_, 0, 0, nullptr);
	}

	WaitForMultipleObjects(workerCount_, hWorkerThread_.data(), TRUE, INFINITE);

	for (int i = 0; i < workerCount_; i++)
	{
		CloseHandle(hWorkerThread_[i]);
	}

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

	// HACK: 이후 수정 필요
	closesocket(session->socket_);

	return true;
}

// lock_guard 사용시, sessionLock이 삭제될 위험 있으므로 사용 안함
bool LanServer::SendPacket(__int64 sessionId, Packet* packet)
{
	PRO(L"SendPacket()");

	sessionMapLock_.lock();

	auto it = sessionMap_.find(sessionId);

	if (it == sessionMap_.end())
	{
		sessionMapLock_.unlock();

		return false;
	}

	Session* session = (*it).second;

	session->sessionLock_.lock();

	sessionMapLock_.unlock();


	///헤더 변경시 수정//////////////////////////////////////////////////////////////////////////////////////
	//packet->GetHeaderPtr()->code_ = PACKET_CODE;

	packet->GetHeaderPtr()->size_ = packet->UseSize();



	//session->sendQueue_.Enqueue(packet);

	session->sendQueue_.Enqueue(packet->GetBufferPtr(), packet->TotalUseSize());



	//session->sessionLock_.unlock();

	delete packet;

	InterlockedIncrement(&sendMessageCount_);

	this->SendPost(session);

	session->sessionLock_.unlock();

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
				wprintf(L"accept() error");
			}
		}

		InterlockedIncrement(&server->acceptCount_);

		CHAR str[16];
		InetNtopA(AF_INET, &clientAddr.sin_addr, str, 16);
		std::string ip = str;
		int port = ntohs(clientAddr.sin_port);

		// 허용되지 않은 ip와 port라면 바로 연결을 끊음
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

		PRO(L"GQCS()");

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
					//__debugbreak();

					continue;
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

		if (((OverlappedEx*)overlapped)->type == IO_TYPE::RECV)
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
		}
	}

	return 0;
}

void LanServer::Monitoring()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// 콘솔창 커서 숨기기
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);

	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &cursorInfo);

	LONG acceptTps = InterlockedExchange(&acceptCount_, 0);
	LONG recvMessageTps = InterlockedExchange(&recvMessageCount_, 0);
	LONG sendMessageTps = InterlockedExchange(&sendMessageCount_, 0);

	system("cls");

	printf("=====================\n");
	printf(" SERVER  MONITORING\n");
	printf("=====================\n\n");

	printf("Acpt TPS : %d\n", acceptTps);
	printf("Recv TPS : %d\n", recvMessageTps);
	printf("Send TPS : %d\n", sendMessageTps);
}
