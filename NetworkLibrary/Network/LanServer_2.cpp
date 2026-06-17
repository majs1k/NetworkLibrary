#include "LanServer.h"
#include "../Utils/Packet.h"
#include "../Utils/Logger.h"
#include "../Utils/Profiler.h"

void LanServer::postRecv(Session* session)
{
	//sessionLock_.lock();

	// 처음에 recv를 등록하기전에 카운트를 증가시킴!!!
	InterlockedIncrement(&session->ioCount_);

	ZeroMemory(&session->recvOverlapped_.overlapped, sizeof(WSAOVERLAPPED));

	//sessionLock_.unlock();

	DWORD flags = 0;
	int retval;

	WSABUF wsaBuf[2];

	//sessionLock_.lock();

	wsaBuf[0].buf = session->recvQueue_.getRearBufferPtr();
	wsaBuf[0].len = session->recvQueue_.directEnqueueSize();

	if (session->recvQueue_.freeSize() == session->recvQueue_.directEnqueueSize())
	{
		// buf 1개
		retval = WSARecv(session->socket_, wsaBuf, 1, nullptr, &flags, (WSAOVERLAPPED*)&session->recvOverlapped_, NULL);
	}
	else
	{
		// buf 2개
		wsaBuf[1].buf = session->recvQueue_.getBufferPtr();
		wsaBuf[1].len = session->recvQueue_.freeSize() - session->recvQueue_.directEnqueueSize();

		retval = WSARecv(session->socket_, wsaBuf, 2, nullptr, &flags, (WSAOVERLAPPED*)&session->recvOverlapped_, NULL);
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
			this->decrementIoCount(session);

			return;
		}
		else
		{
			LOG(L"WSARecv() error: %d, iocount: %d", error, session->ioCount_);
			DebugBreak();
			this->decrementIoCount(session);

			return;
		}
	}
	else
	{
		//printf("recv() Fast IO\n");
	}
}

/// TestServer
void LanServer::completeRecv(Session* session, int numOfBytes)
{
	//sessionLock_.lock();

	session->recvQueue_.moveRear(numOfBytes);

	//sessionLock_.unlock();

	while (1)
	{
		//sessionLock_.lock();

		int useSize = session->recvQueue_.useSize();

		if (session->recvQueue_.isFull())
		{
			//sessionLock_.unlock();

			LOG_INFO(L"recvQueue full");

			//연결 종료 로직 필요

			return;
		}

		//sessionLock_.unlock();

		if (useSize < sizeof(HEADER))
			break;

		HEADER header;

		//sessionLock_.lock();

		session->recvQueue_.peek((char*)&header, sizeof(HEADER));

		//sessionLock_.unlock();

		int messageSize = header.size;

		if (messageSize < 0)
			break;

		if (useSize < sizeof(HEADER) + messageSize)
			break;

		//sessionLock_.lock();

		session->recvQueue_.moveFront(sizeof(HEADER));

		Packet packet;

		session->recvQueue_.dequeue(packet.getBufferPtr(), messageSize);

		//sessionLock_.unlock();

		packet.moveWritePos(messageSize);

		//printf("recvQueue dequeue : %lld\n", *(__int64*)packet.buffer());

		this->onRecv(session->sessionId_, packet);

		InterlockedIncrement(&recvMessageCount_);
	}

	this->postRecv(session);

	this->decrementIoCount(session);
}

/// FighterServer
//void LanServer::completeRecv(Session* session, int numOfBytes)
//{
//	session->recvQueue_.moveRear(numOfBytes);
//
//	while (1)
//	{
//		int useSize = session->recvQueue_.useSize();
//
//		if (session->recvQueue_.isFull())
//		{
//
//			LOG_INFO(L"recvQueue full");
//
//			//연결 종료 로직 필요
//
//			return;
//		}
//
//		if (useSize < sizeof(FIGHTER_HEADER) + sizeof(unsigned char))
//			break;
//
//		FIGHTER_HEADER header;
//
//		session->recvQueue_.peek((char*)&header, sizeof(FIGHTER_HEADER));
//
//		if (header.code != PACKET_CODE)
//		{
//			this->decrementIoCount(session);
//
//			LOG(L"[NETWORK] invalid packet header session=%d", session->sessionId_);
//
//			return;
//		}
//
//		int messageSize = header.size;
//
//		if (messageSize < 0)
//			break;
//
//		if (useSize < sizeof(FIGHTER_HEADER) + sizeof(unsigned char) + messageSize)
//			break;
//
//		session->recvQueue_.moveFront(sizeof(FIGHTER_HEADER));
//
//		Packet packet;
//
//		session->recvQueue_.dequeue(packet.getBufferPtr(), sizeof(unsigned char) + messageSize);
//
//		packet.moveWritePos(sizeof(unsigned char) + messageSize);
//
//		//printf("recvQueue dequeue : %lld\n", *(__int64*)packet.buffer());
//
//		this->onRecv(session->sessionId_, packet);
//
//		InterlockedIncrement(&recvMessageCount_);
//	}
//
//	this->postRecv(session);
//
//	this->decrementIoCount(session);
//}

void LanServer::postSend(Session* session)
{
	//sessionLock_.lock();

	if (InterlockedExchange(&session->sendPending_, 1) == 1)
	{
		//sessionLock_.unlock();

		return;
	}

	// WSASend() 이전에 호출해야, 
	// 다른 스레드에서 send 완료통지로 ioCount 내려서 0이 되는 상황 차단
	InterlockedIncrement(&session->ioCount_);

	ZeroMemory(&session->sendOverlapped_.overlapped, sizeof(WSAOVERLAPPED));

	//sessionLock_.unlock();

	int retval;

	WSABUF wsaBuf[2];

	//sessionLock_.lock();

	if (session->sendQueue_.useSize() <= session->sendQueue_.directDequeueSize())
	{
		// buf 1개
		wsaBuf[0].buf = session->sendQueue_.getFrontBufferPtr();
		wsaBuf[0].len = session->sendQueue_.useSize();

		//PRO_BEGIN(L"send 1");
		//PRO_BEGIN(L"send 2");
		retval = WSASend(session->socket_, wsaBuf, 1, nullptr, 0, (WSAOVERLAPPED*)&(session->sendOverlapped_), NULL);
		//PRO_END(L"send 1");
	}
	else
	{
		// buf 2개
		wsaBuf[0].buf = session->sendQueue_.getFrontBufferPtr();
		wsaBuf[0].len = session->sendQueue_.directEnqueueSize();

		wsaBuf[1].buf = session->sendQueue_.getBufferPtr();
		wsaBuf[1].len = session->sendQueue_.useSize() - session->sendQueue_.directEnqueueSize();

		//PRO_BEGIN(L"send 1");
		//PRO_BEGIN(L"send 2");
		retval = WSASend(session->socket_, wsaBuf, 2, nullptr, 0, (WSAOVERLAPPED*)&(session->sendOverlapped_), NULL);
		//PRO_END(L"send 1");
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
			this->decrementIoCount(session);

			return;
		}
		else
		{
			LOG(L"WSASend() error: %d, iocount: %d", error, session->ioCount_);
			DebugBreak();
			this->decrementIoCount(session);

			return;
		}
	}
	else
	{
		//printf("send Fast IO\n");
	}
}

void LanServer::completeSend(Session* session, int numOfBytes)
{
	//PRO_END(L"send 2");

	//sessionLock_.lock();

	session->sendQueue_.moveFront(numOfBytes);

	//session->sendPending_ = 0;
	InterlockedExchange(&session->sendPending_, 0);

	if (session->sendQueue_.useSize() > 0)
		this->postSend(session);

	//sessionLock_.unlock();

	this->decrementIoCount(session);
}

void LanServer::decrementIoCount(Session* session)
{
	if (InterlockedDecrement(&session->ioCount_) == 0)
	{
		this->disconnect(session->sessionId_);

		return;
	}
}
