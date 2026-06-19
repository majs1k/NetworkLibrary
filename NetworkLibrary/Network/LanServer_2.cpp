#include "LanServer.h"
#include "Session.h"
#include "../Utils/Packet.h"
#include "../Utils/Logger.h"
#include "../Utils/Profiler.h"

/// ver1
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

/// ver2
//void LanServer::postRecv(Session* session)
//{
//	InterlockedIncrement(&session->ioCount_);
//
//	ZeroMemory(&session->recvOverlapped_.overlapped, sizeof(WSAOVERLAPPED));
//
//
//	DWORD flags = 0;
//	int retval;
//
//	WSABUF wsaBuf;
//
//	PacketT<Buffer*> p;
//	/// p의 refCount 증가??
//	
//	Buffer* buffer = new Buffer(500);
//	buffer->increase(1);
//
//	p.enqueue(buffer);
//
//	wsaBuf.buf = p.getBufferPtr();
//	wsaBuf.len = 500;
//
//	retval = WSARecv(session->socket_, &wsaBuf, 1, nullptr, &flags, (WSAOVERLAPPED*)&session->recvOverlapped_, NULL);
//	
//	if (retval == SOCKET_ERROR)
//	{
//		int error = WSAGetLastError();
//
//		if (error == ERROR_IO_PENDING)
//		{
//			//printf("recv() Direct IO\n");
//		}
//		else if (error == WSAECONNRESET || error == WSAECONNABORTED)
//		{
//			this->decrementIoCount(session);
//
//			return;
//		}
//		else
//		{
//			LOG(L"WSARecv() error: %d, iocount: %d", error, session->ioCount_);
//			DebugBreak();
//			this->decrementIoCount(session);
//
//			return;
//		}
//	}
//	else
//	{
//		//printf("recv() Fast IO\n");
//	}
//}

/// TestServer ver1
//void LanServer::completeRecv(Session* session, int numOfBytes)
//{
//	//sessionLock_.lock();
//
//	session->recvQueue_.moveRear(numOfBytes);
//
//	//sessionLock_.unlock();
//
//	while (1)
//	{
//		//sessionLock_.lock();
//
//		int useSize = session->recvQueue_.useSize();
//
//		if (session->recvQueue_.isFull())
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
//		session->recvQueue_.peek((char*)&header, sizeof(HEADER));
//
//		//sessionLock_.unlock();
//
//		int messageSize = header.size;
//
//		if (messageSize < 0)
//			break;
//
//		if (useSize < sizeof(HEADER) + messageSize)
//			break;
//
//		//sessionLock_.lock();
//
//		session->recvQueue_.moveFront(sizeof(HEADER));
//
//		Packet packet;
//
//		session->recvQueue_.dequeue(packet.getBufferPtr(), messageSize);
//
//		//sessionLock_.unlock();
//
//		packet.moveWritePos(messageSize);
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

/// TestServer ver2
void LanServer::completeRecv(Session* session, int numOfBytes)
{
	session->recvQueue_.moveRear(numOfBytes);

	while (1)
	{
		int useSize = session->recvQueue_.useSize();

		if (session->recvQueue_.isFull())
		{
			LOG_INFO(L"recvQueue full");

			//연결 종료 로직 필요

			return;
		}

		if (useSize < sizeof(HEADER))
			break;

		HEADER header;

		session->recvQueue_.peek((char*)&header, sizeof(HEADER));

		int messageSize = header.size_;

		if (messageSize < 0)
			break;

		if (useSize < sizeof(HEADER) + messageSize)
			break;

		session->recvQueue_.moveFront(sizeof(HEADER));

		Packet packet;

		session->recvQueue_.dequeue(packet.getPacketPtr(), messageSize);

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

/// ver1
//void LanServer::postSend(Session* session)
//{
//	//sessionLock_.lock();
//
//	if (InterlockedExchange(&session->sendPending_, 1) == 1)
//	{
//		//sessionLock_.unlock();
//
//		return;
//	}
//
//	// WSASend() 이전에 호출해야, 
//	// 다른 스레드에서 send 완료통지로 ioCount 내려서 0이 되는 상황 차단
//	InterlockedIncrement(&session->ioCount_);
//
//	ZeroMemory(&session->sendOverlapped_.overlapped, sizeof(WSAOVERLAPPED));
//
//	//sessionLock_.unlock();
//
//	int retval;
//
//	WSABUF wsaBuf[2];
//
//	//sessionLock_.lock();
//
//	if (session->sendQueue_.useSize() <= session->sendQueue_.directDequeueSize())
//	{
//		// buf 1개
//		wsaBuf[0].buf = session->sendQueue_.getFrontBufferPtr();
//		wsaBuf[0].len = session->sendQueue_.useSize();
//
//		//PRO_BEGIN(L"send 1");
//		//PRO_BEGIN(L"send 2");
//		retval = WSASend(session->socket_, wsaBuf, 1, nullptr, 0, (WSAOVERLAPPED*)&(session->sendOverlapped_), NULL);
//		//PRO_END(L"send 1");
//	}
//	else
//	{
//		// buf 2개
//		wsaBuf[0].buf = session->sendQueue_.getFrontBufferPtr();
//		wsaBuf[0].len = session->sendQueue_.directEnqueueSize();
//
//		wsaBuf[1].buf = session->sendQueue_.getBufferPtr();
//		wsaBuf[1].len = session->sendQueue_.useSize() - session->sendQueue_.directEnqueueSize();
//
//		//PRO_BEGIN(L"send 1");
//		//PRO_BEGIN(L"send 2");
//		retval = WSASend(session->socket_, wsaBuf, 2, nullptr, 0, (WSAOVERLAPPED*)&(session->sendOverlapped_), NULL);
//		//PRO_END(L"send 1");
//	}
//
//	//sessionLock_.unlock();
//
//	if (retval == SOCKET_ERROR)
//	{
//		int error = WSAGetLastError();
//
//		if (error == ERROR_IO_PENDING)
//		{
//			//printf("send Direct IO\n");
//		}
//		else if (error == WSAECONNRESET || error == WSAECONNABORTED)
//		{
//			this->decrementIoCount(session);
//
//			return;
//		}
//		else
//		{
//			LOG(L"WSASend() error: %d, iocount: %d", error, session->ioCount_);
//			DebugBreak();
//			this->decrementIoCount(session);
//
//			return;
//		}
//	}
//	else
//	{
//		//printf("send Fast IO\n");
//	}
//}

#define MAX_WSABUF 50

/// ver2
void LanServer::postSend(Session* session)
{
	if (InterlockedExchange(&session->sendPending_, 1) == 1)
		return;

	// WSASend() 이전에 호출해야, 
	// 다른 스레드에서 send 완료통지로 ioCount 내려서 0이 되는 상황 차단
	InterlockedIncrement(&session->ioCount_);

	ZeroMemory(&session->sendOverlapped_.overlapped, sizeof(WSAOVERLAPPED));

	int retval;

	WSABUF wsaBuf[MAX_WSABUF];

	session->sendPacketCount_ = 0;

	session->sendQueue2_.lock();

	int n = session->sendQueue2_.useSize();

	// wsasend() 호출시 wasbuf count 0인 상황 발생시 10022 에러 발생함. 예외처리
	if (n <= 0)
	{
		session->sendQueue2_.unlock();

		InterlockedExchange(&session->sendPending_, 0);

		return;
	}

	for (int i = 0; i < n && i < MAX_WSABUF; i++)
	{
		Packet* packet = nullptr;

		session->sendQueue2_.dequeue(packet);

		wsaBuf[i].buf = reinterpret_cast<char*>(packet->getCompletePtr());
		wsaBuf[i].len = packet->completeSize();

		//if (p->useSize() > 2)
		//{
		//	printf("%lld\n", *(__int64*)wsaBuf[i].buf);
		//}
		//else
		//{
		//	printf("%d\n", *(unsigned short*)wsaBuf[i].buf);
		//}

		session->sendPacketCount_++;
	}

	//if (session->sendPacketCount_ <= 0)
	//{
	//	session->sendPackets_.unlock();

	//	InterlockedExchange(&session->sendPending_, 0);

	//	return;
	//}

	session->sendQueue2_.moveFrontReverse(session->sendPacketCount_);

	session->sendQueue2_.unlock();

	// wsaBuf count가 0이면 10022 에러 발생함
	retval = WSASend(session->socket_, wsaBuf, session->sendPacketCount_, nullptr, 0, (WSAOVERLAPPED*)&(session->sendOverlapped_), NULL);

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
			LOG(L"WSASend() error: %d", error);
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

/// ver1
//void LanServer::completeSend(Session* session, int numOfBytes)
//{
//	//PRO_END(L"send 2");
//
//	//sessionLock_.lock();
//
//	session->sendQueue_.moveFront(numOfBytes);
//
//	//session->sendPending_ = 0;
//	InterlockedExchange(&session->sendPending_, 0);
//
//	if (session->sendQueue_.useSize() > 0)
//		this->postSend(session);
//
//	//sessionLock_.unlock();
//
//	this->decrementIoCount(session);
//}


/// ver2
void LanServer::completeSend(Session* session, int numOfBytes)
{
	Packet* p = nullptr;

	session->sendQueue2_.lock();

	// numOfBytes는 신경 쓰지말고, sendPacketCount_로 처리
	for (int i = 0; i < session->sendPacketCount_; i++)
	{
		session->sendQueue2_.dequeue(p);

		this->decrementPacketCount(p);
	}

	// 여기선 0으로 봤는데 밑에서 size > 0 확인할떄 0이 아니면 send를 못하는 문제 발생가능
	//int size = session->sendPackets_.useSize();

	session->sendQueue2_.unlock();

	InterlockedExchange(&session->sendPending_, 0);

	session->sendQueue2_.lock();

	if (session->sendQueue2_.useSize() > 0)
		this->postSend(session);

	session->sendQueue2_.unlock();

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

void LanServer::decrementPacketCount(Packet* packet)
{
	if (packet->decrease() == 0)
	{
		delete packet;

		return;
	}
}
