#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

#include <process.h>
#include "LanClient.h"
#include "../Utils/Logger.h"
#include "../Utils/Profiler.h"

LanClient::LanClient()
{
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		ERR(L"WSAStartup() error");
}

LanClient::~LanClient()
{
	WSACleanup();
}

bool LanClient::Connect()
{
	serverIp_ = L"0.0.0.0";
	serverPort_ = SERVER_PORT;

	hIOCP_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, CONCURRENT_COUNT);

	for (int i = 0; i < WORKER_COUNT; i++)
	{
		hWorkerThread_[i] = (HANDLE)_beginthreadex(nullptr, 0, WorkerThread, this, 0, nullptr);
	}

	clientSocket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket_ == INVALID_SOCKET)
		ERR(L"socket() error");

	// 링거 옵션 - closesocekt() 호출시 즉시 리턴, 연결 강제 종료
	// 서버 클라 둘다 설정할 것
	LINGER optval;
	optval.l_onoff = 1;
	optval.l_linger = 0;

	if (setsockopt(clientSocket_, SOL_SOCKET, SO_LINGER, (char*)&optval, sizeof(optval)) == SOCKET_ERROR)
		ERR(L"setsockopt() error");

	// L4 송신버퍼 사이즈 옵션
	int sndBufSize = 0;
	//setsockopt(clientSocket_, SOL_SOCKET, SO_SNDBUF, (const char*)(&sndBufSize), sizeof(sndBufSize));

	int size = 0;
	int len = sizeof(size);
	getsockopt(clientSocket_, SOL_SOCKET, SO_SNDBUF, (char*)(&size), &len);

	std::cout << "SO_SNDBUF = " << size << std::endl;

	SOCKADDR_IN clientAddr;
	ZeroMemory(&clientAddr, sizeof(clientAddr));
	clientAddr.sin_family = AF_INET;
	InetPton(AF_INET, serverIp_.c_str(), &clientAddr.sin_addr);
	clientAddr.sin_port = htons(serverPort_);

	if (::connect(clientSocket_, (SOCKADDR*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error == WSAENOTSOCK)
		{
			//LOG_INFO(L"[NETWORK] accept thread exit");

			return false;
		}
		else
		{
			ERR(L"connect() error!");
		}
	}

	// connect 이후 논블로킹 소켓으로 전환
	//u_long on = 1;
	//if (ioctlsocket(clientSocket_, FIONBIO, &on) == SOCKET_ERROR)
	//	ERR(L"iocltsocket error!");

	clientSession_ = new Session();
	clientSession_->Initialize(clientSocket_, serverIp_, serverPort_, 1);

	LOG_INFO(L"[NETWORK] create client");

	CreateIoCompletionPort((HANDLE)clientSocket_, hIOCP_, (ULONG_PTR)clientSession_, 0);

	OnEnterJoinServer();

	this->RecvPost(clientSession_);
}

bool LanClient::Disconnect()
{
	closesocket(clientSocket_);

	for (int i = 0; i < WORKER_COUNT; i++)
	{
		PostQueuedCompletionStatus(hIOCP_, 0, 0, nullptr);
	}

	WaitForMultipleObjects(WORKER_COUNT, hWorkerThread_, TRUE, INFINITE);

	for (int i = 0; i < WORKER_COUNT; i++)
	{
		CloseHandle(hWorkerThread_[i]);
	}

	delete clientSession_;

	CloseHandle(hIOCP_);

	LOG_INFO(L"[NETWORK] client exit");

	return true;
}

bool LanClient::SendPacket(Packet& packet)
{




	return true;
}

unsigned int __stdcall LanClient::WorkerThread(void* param)
{
	LanClient* client = (LanClient*)param;

	DWORD numOfBytes;
	Session* session;
	OVERLAPPED* overlapped;

	while (1)
	{
		//session = nullptr;
		//numOfBytes = 0;

		// GQCS 호출 반환시 overlapped 구조체 무조건 세팅됨
		BOOL ret = GetQueuedCompletionStatus(client->hIOCP_, &numOfBytes, (PULONG_PTR)&session, &overlapped, INFINITE);

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
					client->DecrementIoCount(session);

					continue;
				}
				// 1236 clientSock 닫혓을때
				// ioCount 도입했기 때문에 뜨면 안됨
				else if (error == ERROR_CONNECTION_ABORTED)
				{
					LOG_INFO(L"GQCS() error: ERROR_CONNECTION_ABORTED");

					client->DecrementIoCount(session);

					continue;
				}

				LOG_INFO(L"GQCS() error: %d", error);

				client->DecrementIoCount(session);

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
				client->DecrementIoCount(session);

				continue;
			}

			client->CompleteRecv(session, numOfBytes);
		}
		else
		{
			client->CompleteSend(session, numOfBytes);
		}
	}

	return 0;
}
