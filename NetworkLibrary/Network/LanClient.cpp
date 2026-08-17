#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

#include "LanClient.h"
#include "../Utils/Packet.h"
#include "../Utils/Logger.h"
#include "../Utils/Profiler.h"

LanClient::LanClient()
{
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		wprintf(L"WSAStartup() error");
}

LanClient::~LanClient()
{
	delete session_;

	WSACleanup();
}

bool LanClient::Connect(std::wstring ip, int port)
{
	SOCKET clientSocket_ = socket(PF_INET, SOCK_STREAM, 0);
	if (clientSocket_ == INVALID_SOCKET)
		wprintf(L"socket() error!");

	// 링거 옵션
	LINGER optval;
	optval.l_onoff = 1;
	optval.l_linger = 0;
	if (setsockopt(clientSocket_, SOL_SOCKET, SO_LINGER, (char*)&optval, sizeof(optval)) == SOCKET_ERROR)
		wprintf(L"setsockopt() error!");

	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	InetPton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

	if (connect(clientSocket_, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		wprintf(L"connect() error!");

	// connect 이후 논블로킹 옵션
	u_long on = 1;
	if (ioctlsocket(clientSocket_, FIONBIO, &on) == SOCKET_ERROR)
		wprintf(L"iocltsocket error!");


	session_ = new CSession();
	// 세션 초기화
	session_->socket_ = clientSocket_;
	session_->ip_ = ip;
	session_->port_ = port;

	return true;
}

bool LanClient::Disconnect()
{

	return true;
}

// lock_guard 사용시, sessionLock이 삭제될 위험 있으므로 사용 안함
bool LanClient::SendPacket(Packet* packet)
{
	packet->GetHeaderPtr()->size_ = packet->UseSize();

	session_->sendQueue_.Enqueue(packet->GetBufferPtr(), packet->TotalUseSize());

	delete packet;

	return true;
}

void LanClient::ProcessNetwork()
{
	if (session_->sendQueue_.UseSize() > 0)
		SendPost();

	RecvPost();
}

void LanClient::RecvPost()
{
	// 발전 단계
	// 1. 로컬버퍼로 recv
	// 2. 링버퍼 추가
	// 3. 로컬버퍼 스킵 후 링버퍼로 바로 인큐 디큐

	//char buf[RECV_SIZE];
	//int recvLen = ::recv(socket_, buf, RECV_SIZE, 0);

	int recvLen = recv(session_->socket_, session_->recvQueue_.GetRearBufferPtr(), session_->recvQueue_.DirectEnqueueSize(), 0);

	if (recvLen == SOCKET_ERROR)
	{
		int error = WSAGetLastError();

		if (error == WSAEWOULDBLOCK)
		{
			//wprintf(L"recv() wouldblock\n");

			return;
		}
		else if (error == WSAECONNRESET)
		{
			Disconnect();
			return;
		}
		else
		{
			wprintf(L"recv() error : %d\n", error);
			return;
		}
	}
	else if (recvLen == 0)
	{
		Disconnect();
		return;
	}

	CompleteRecv(recvLen);
}

void LanClient::CompleteRecv(int numOfBytes)
{
	session_->recvQueue_.MoveRear(numOfBytes);

	while (1)
	{
		int useSize = session_->recvQueue_.UseSize();

		if (session_->recvQueue_.IsFull())
		{
			LOG_INFO(L"recvQueue full");

			//연결 종료 로직

			return;
		}

		if (useSize < sizeof(UNITY_HEADER))
			break;

		UNITY_HEADER header;

		session_->recvQueue_.Peek((char*)&header, sizeof(UNITY_HEADER));

		int messageSize = header.size_;

		if (messageSize < 0)
			break;

		if (useSize < sizeof(UNITY_HEADER) + messageSize)
			break;

		//session->recvQueue_.MoveFront(sizeof(TEST_HEADER));

		Packet* packet = new Packet();
		packet->Initialize();

		session_->recvQueue_.Dequeue(packet->GetBufferPtr(), messageSize + sizeof(UNITY_HEADER));

		packet->MoveWritePos(messageSize);

		this->OnRecv(packet);
	}
}

void LanClient::SendPost()
{
	// 지역 버퍼 사용 안하는 방식으로 수정
	//int sendLen = ::send(socket_, buf, len, 0);

	int sendLen = send(session_->socket_, session_->sendQueue_.GetFrontBufferPtr(), session_->sendQueue_.DirectDequeueSize(), 0);

	if (sendLen == SOCKET_ERROR)
	{
		int error = WSAGetLastError();

		if (error == WSAEWOULDBLOCK)
		{
			wprintf(L"send() wouldblock\n");

			return;
		}
		else if (error == WSAECONNRESET)
		{
			Disconnect();
			return;
		}
		else
		{
			wprintf(L"send() error %d\n", error);
			return;
		}
	}

	session_->sendQueue_.MoveFront(sendLen);
}
