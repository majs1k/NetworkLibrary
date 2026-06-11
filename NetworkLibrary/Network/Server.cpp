#pragma comment(lib, "ws2_32.lib")
#include <process.h>
#include <WS2tcpip.h>
#include "Server.h"	
#include "Session.h"
#include "SessionManager.h"
#include "../Utils/Logger.h"
#include "../Utils/Profiler.h"

auto& sm = SessionManager::getInstance();

Server::Server()
{
	ip_ = L"0.0.0.0";
	port_ = SERVER_PORT;

	WSADATA wsa;
	//ip_ = ConfigManager::getInstance().network_.ip;
	//port_ = ConfigManager::getInstance().network_.port;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		ERR(L"WSAStartup() error");
}

Server::~Server()
{
	WSACleanup();
}

void Server::initialize()
{
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

	LOG_INFO(L"[NETWORK] server start");
}

void Server::serverExit()
{
	closesocket(listenSocket_);

	for (int i = 0; i < WORKER_NUM; i++)
	{
		PostQueuedCompletionStatus(hIOCP_, 0, 0, nullptr);
	}

	WaitForSingleObject(hAcceptThread_, INFINITE);
	WaitForMultipleObjects(WORKER_NUM, hWorkerThread_, TRUE, INFINITE);

	CloseHandle(hAcceptThread_);

	for (int i = 0; i < WORKER_NUM; i++)
	{
		CloseHandle(hWorkerThread_[i]);
	}

	// 세션맵 정리??

	CloseHandle(hIOCP_);

	LOG_INFO(L"[NETWORK] server exit");
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
		server->onAccept(clientSock, clientAddr);
	}

	return 0;
}

void Server::onAccept(SOCKET socket, SOCKADDR_IN sockAddr)
{
	// 세션 생성 전에 최대세션 확인?
	//if (SessionManager::getInstance().isFull())
	//{
	//	closesocket(socket);
	//	LOG(L"[Network] session limit over");
	//	return;
	//}

	wchar_t str[16];
	InetNtop(AF_INET, &sockAddr.sin_addr, str, 16);
	std::wstring ip = str;

	// TODO: 접속된 ip 확인
	// 해외 ip 차단
	// 서버 공격은 LAN 외부 ip 기준일까?
	// 아니면 더미는 내 ip니까 그냥 모든 외부 ip에 연결 제한을 두는 개념?

	int port = ntohs(sockAddr.sin_port);

	Session* session = new Session();
	session->initialize(socket, ip, port, SessionManager::getInstance().requireId());

	SessionManager::getInstance().addSession(session);

	CreateIoCompletionPort((HANDLE)socket, hIOCP_, (ULONG_PTR)session, 0);

	session->postRecv();
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

		InterlockedIncrement(&cnt);

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
			if (numOfBytes == 0)
			{
				/// 발생 확인 필요
				LOG_INFO(L"recv 0byte");
				session->decrementIOCount();
				
				continue;
			}

			session->completeRecv(numOfBytes);
		}
		else
		{
			session->completeSend(numOfBytes);
		}

		InterlockedDecrement(&cnt);
	}

	return 0;
}
