#include "Session.h"	
#include "LanServer.h"
#include "../Utils/Packet.h"
#include "../Utils/Logger.h"

Session::~Session()
{
	closesocket(socket_);
}

void Session::initialize(SOCKET socket, std::wstring ip, int port, __int64 id)
{
	socket_ = socket;
	sessionId_ = id;

	ip_ = ip;
	port_ = port;

	sendQueue_.clear();
	recvQueue_.clear();

	sendOverlapped_.type = IOType::SEND;
	recvOverlapped_.type = IOType::RECV;

	ioCount_ = 0;
	sendPending_ = false;
}

SOCKET Session::socket() const
{
	return socket_;
}

__int64 Session::id() const
{
	return sessionId_;
}

std::wstring Session::ip() const
{
	return ip_;
}

int Session::port() const
{
	return port_;
}

void Session::postRecv()
{
	//sessionLock_.lock();

	// 처음에 recv를 등록하기전에 카운트를 증가시킴!!!
	InterlockedIncrement(&ioCount_);

	ZeroMemory(&recvOverlapped_.overlapped, sizeof(WSAOVERLAPPED));

	//sessionLock_.unlock();

	DWORD flags = 0;
	int retval;

	WSABUF wsaBuf[2];

	//sessionLock_.lock();

	wsaBuf[0].buf = recvQueue_.getRearBufferPtr();
	wsaBuf[0].len = recvQueue_.directEnqueueSize();

	if (recvQueue_.freeSize() == recvQueue_.directEnqueueSize())
	{
		// buf 1개
		retval = WSARecv(socket_, wsaBuf, 1, nullptr, &flags, (WSAOVERLAPPED*)&recvOverlapped_, NULL);
	}
	else
	{
		// buf 2개
		wsaBuf[1].buf = recvQueue_.getBufferPtr();
		wsaBuf[1].len = recvQueue_.freeSize() - recvQueue_.directEnqueueSize();

		retval = WSARecv(socket_, wsaBuf, 2, nullptr, &flags, (WSAOVERLAPPED*)&recvOverlapped_, NULL);
	}

	//sessionLock_.unlock();

	if (retval == SOCKET_ERROR)
	{
		int error = WSAGetLastError();

		if (error == ERROR_IO_PENDING)
		{
			//printf("recv() Direct IO\n");
		}
		else if (error == WSAECONNRESET || error == WSAECONNABORTED)
		{
			this->decrementIoCount();

			return;
		}
		else
		{
			LOG(L"WSARecv() error: %d, iocount: %d", error, ioCount_);

			this->decrementIoCount();

			return;
		}
	}
	else
	{
		//printf("recv() Fast IO\n");
	}
}

/// TestServer
//void Session::completeRecv(int numOfBytes)
//{
//	//printf("completeRecv() %d\n", numOfBytes);
//	sessionLock_.lock();
//
//	recvQueue_.moveRear(numOfBytes);
//
//	sessionLock_.unlock();
//
//	while (1)
//	{
//		//sessionLock_.lock();
//
//		int useSize = recvQueue_.useSize();
//
//		if (recvQueue_.isFull())
//		{
//			//sessionLock_.unlock();
//
//			LOG_INFO(L"recvQueue full");
//
//			//연결 종료 로직 필요
//
//			return;
//		}
//
//		//sessionLock_.unlock();
//
//		if (useSize < sizeof(HEADER))
//			break;
//
//		HEADER header;
//
//		//sessionLock_.lock();
//
//		recvQueue_.peek((char*)&header, sizeof(HEADER));
//
//		//sessionLock_.unlock();
//
//		int messageSize = header.size;
//
//		if (useSize < sizeof(HEADER) + messageSize)
//			break;
//
//		//sessionLock_.lock();
//
//		recvQueue_.moveFront(sizeof(HEADER));
//
//		Packet packet;
//
//		recvQueue_.dequeue(packet.getBufferPtr(), messageSize);
//
//		//sessionLock_.unlock();
//
//		packet.moveWritePos(messageSize);
//
//		//printf("recvQueue dequeue : %lld\n", *(__int64*)packet.buffer());
//
//		server->onRecv(sessionId_, packet);
//
//		server->increaseRecvMessageTps();
//	}
//
//	this->postRecv();
//
//	this->decrementIoCount();
//}


#include "../RPC/RPCStub.h"
#include <iostream>

/// FighterServer
void Session::completeRecv(int numOfBytes)
{
	//printf("completeRecv() %d\n", numOfBytes);
	//sessionLock_.lock();

	recvQueue_.moveRear(numOfBytes);

	//sessionLock_.unlock();

	while (1)
	{
		//sessionLock_.lock();

		int useSize = recvQueue_.useSize();

		if (recvQueue_.isFull())
		{
			//sessionLock_.unlock();

			LOG_INFO(L"recvQueue full");

			//연결 종료 로직 필요

			return;
		}

		//sessionLock_.unlock();

		if (useSize < sizeof(FIGHTER_HEADER) + sizeof(unsigned char))
			break;

		FIGHTER_HEADER header;

		//sessionLock_.lock();

		recvQueue_.peek((char*)&header, sizeof(FIGHTER_HEADER));

		//sessionLock_.unlock();

		if (header.code != PACKET_CODE)
		{
			this->decrementIoCount();

			LOG(L"[NETWORK] invalid packet header session=%d", sessionId_);

			return;
		}

		int messageSize = header.size;

		if (useSize < sizeof(FIGHTER_HEADER) + sizeof(unsigned char) + messageSize)
			break;

		//sessionLock_.lock();

		recvQueue_.moveFront(sizeof(FIGHTER_HEADER));

		Packet packet;

		recvQueue_.dequeue(packet.getBufferPtr(), sizeof(unsigned char) + messageSize);

		//sessionLock_.unlock();

		packet.moveWritePos(sizeof(unsigned char) + messageSize);

		//printf("recvQueue dequeue : %lld\n", *(__int64*)packet.buffer());

		server->onRecv(sessionId_, packet);
	}

	this->postRecv();

	this->decrementIoCount();
}

void Session::postSend()
{
	//sessionLock_.lock();

	if (InterlockedExchange(&sendPending_, 1) == 1)
	{
		//sessionLock_.unlock();

		return;
	}

	// WSASend() 이전에 호출해야, 
	// 다른 스레드에서 send 완료통지로 ioCount 내려서 0이 되는 상황 차단
	InterlockedIncrement(&ioCount_);

	ZeroMemory(&sendOverlapped_.overlapped, sizeof(WSAOVERLAPPED));

	//sessionLock_.unlock();

	int retval;

	WSABUF wsaBuf[2];

	//sessionLock_.lock();

	if (sendQueue_.useSize() <= sendQueue_.directDequeueSize())
	{
		// buf 1개
		wsaBuf[0].buf = sendQueue_.getFrontBufferPtr();
		wsaBuf[0].len = sendQueue_.useSize();

		//PRO_BEGIN(L"send");
		retval = WSASend(socket_, wsaBuf, 1, nullptr, 0, (WSAOVERLAPPED*)&sendOverlapped_, NULL);
		//PRO_END(L"send");
	}
	else
	{
		// buf 2개
		wsaBuf[0].buf = sendQueue_.getFrontBufferPtr();
		wsaBuf[0].len = sendQueue_.directEnqueueSize();

		wsaBuf[1].buf = sendQueue_.getBufferPtr();
		wsaBuf[1].len = sendQueue_.useSize() - sendQueue_.directEnqueueSize();

		//PRO_BEGIN(L"send");
		retval = WSASend(socket_, wsaBuf, 2, nullptr, 0, (WSAOVERLAPPED*)&sendOverlapped_, NULL);
		//PRO_END(L"send");
	}

	//sessionLock_.unlock();

	if (retval == SOCKET_ERROR)
	{
		int error = WSAGetLastError();

		if (error == ERROR_IO_PENDING)
		{
			//printf("send Direct IO\n");
		}
		else if (error == WSAECONNRESET || error == WSAECONNABORTED)
		{
			this->decrementIoCount();

			return;
		}
		else
		{
			LOG(L"WSASend() error: %d, iocount: %d", error, ioCount_);

			this->decrementIoCount();

			return;
		}
	}
	else
	{
		//printf("send Fast IO\n");
	}
}

void Session::completeSend(int numOfBytes)
{
	//sessionLock_.lock();

	sendQueue_.moveFront(numOfBytes);

	sendPending_ = 0;

	// 추가
	int useSize = sendQueue_.useSize();

	//sessionLock_.unlock();

	if (useSize > 0)
		this->postSend();

	//printf("sendq usesize: %d\n", useSize);

	this->decrementIoCount();
}

/// TestServer
//void Session::sendPacket(Packet& packet)
//{
//	//if (ioCount_ == 0)
//	//	return;
//
//	//if(InterlockedIncrement(&ioCount_) == 1)
//	//	return;
//
//	HEADER header;
//	header.size = packet.useSize();
//
//	//sessionLock_.lock();
//
//	sendQueue_.lock();
//
//	sendQueue_.enqueue((char*)&header, sizeof(HEADER));
//	sendQueue_.enqueue(packet.getBufferPtr(), packet.useSize());
//
//	sendQueue_.unlock();
//
//	//sessionLock_.unlock();
//
//	this->postSend();
//}

/// FighterServer
void Session::sendPacket(Packet& packet)
{
	FIGHTER_HEADER header;
	header.code = PACKET_CODE;
	header.size = packet.useSize() - sizeof(unsigned char);

	//sessionLock_.lock();

	sendQueue_.lock();

	sendQueue_.enqueue((char*)&header, sizeof(HEADER));
	sendQueue_.enqueue(packet.getBufferPtr(), packet.useSize());

	sendQueue_.unlock();

	//sessionLock_.unlock();

	this->postSend();
}

void Session::decrementIoCount()
{
	if (InterlockedDecrement(&ioCount_) == 0)
	{
		server->disconnect(sessionId_);

		return;
	}
}
