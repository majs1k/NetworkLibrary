#include "LanServer.h"
#include "Session.h"
#include "../Utils/Packet.h"
#include "../Utils/SendPacket.h"
#include "../Utils/RecvPacket.h"
#include "../Utils/Logger.h"
#include "../Utils/Profiler.h"

/// ver1
void LanServer::recvPost(Session* session)
{
	//PRO(L"postRecv 1");

	ZeroMemory(&session->recvOverlapped_.overlapped, sizeof(WSAOVERLAPPED));

	WSABUF wsaBuf[2];

	wsaBuf[0].buf = session->recvQueue_.getRearBufferPtr();
	wsaBuf[0].len = session->recvQueue_.directEnqueueSize();

	DWORD flags = 0;
	int retval;

	if (session->recvQueue_.freeSize() == session->recvQueue_.directEnqueueSize())
	{
		// WSARecv() 이전에 호출
		this->incrementIoCount(session);

		// buf 1개
		retval = WSARecv(session->socket_, wsaBuf, 1, nullptr, &flags, (WSAOVERLAPPED*)&session->recvOverlapped_, NULL);
	}
	else
	{
		this->incrementIoCount(session);

		// buf 2개
		wsaBuf[1].buf = session->recvQueue_.getBufferPtr();
		wsaBuf[1].len = session->recvQueue_.freeSize() - session->recvQueue_.directEnqueueSize();

		retval = WSARecv(session->socket_, wsaBuf, 2, nullptr, &flags, (WSAOVERLAPPED*)&session->recvOverlapped_, NULL);
	}

	if (retval == SOCKET_ERROR)
	{
		int error = WSAGetLastError();

		if (error == ERROR_IO_PENDING)
		{
			//printf("recv() Direct IO\n");
		}
		else if (error == WSAECONNRESET || error == WSAECONNABORTED || error == WSAENOTSOCK)
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
//void LanServer::recvPost(Session* session)
//{
//	PRO(L"postRecv 2");
//
//	ZeroMemory(&session->recvOverlapped_.overlapped, sizeof(WSAOVERLAPPED));
//
//	RecvPacket* packet = new RecvPacket();
//	PacketBuffer* buffer = new PacketBuffer();
//	packet->initialize(buffer);
//
//	if (session->recvPacket_->useSize() > 0)
//	{
//		packet->write(session->recvPacket_->getReadBufferPtr(), session->recvPacket_->useSize());
//	}
//
//	decrementRecvPacketRefCount(session->recvPacket_);
//
//	session->recvPacket_ = packet;
//
//	WSABUF wsaBuf;
//
//	wsaBuf.buf = session->recvPacket_->getWriteBufferPtr();
//	wsaBuf.len = session->recvPacket_->freeSize();
//
//	DWORD flags = 0;
//	int retval;
//
//	// WSARecv() 이전에 호출
//	this->incrementIoCount(session);
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

/// ver1
void LanServer::completeRecv(Session* session, int numOfBytes)
{
	//PRO(L"completeRecv 1");

	session->recvQueue_.moveRear(numOfBytes);

	while (1)
	{
		int useSize = session->recvQueue_.useSize();

		if (session->recvQueue_.isFull())
		{
			LOG_INFO(L"recvQueue full");

			//연결 종료 로직

			return;
		}

		if (useSize < sizeof(TEST_HEADER))
			break;

		TEST_HEADER header;

		session->recvQueue_.peek((char*)&header, sizeof(TEST_HEADER));

		int messageSize = header.size_;

		if (messageSize < 0)
			break;

		if (useSize < sizeof(TEST_HEADER) + messageSize)
			break;

		session->recvQueue_.moveFront(sizeof(TEST_HEADER));

		Packet* packet = new Packet();

		session->recvQueue_.dequeue(packet->getBufferPtr(), messageSize);

		packet->moveWritePos(messageSize);

		//printf("recvQueue dequeue : %lld\n", *(__int64*)packet.buffer());

		this->onRecv(session->sessionId_, packet);

		InterlockedIncrement(&recvMessageCount_);
	}

	this->recvPost(session);

	this->decrementIoCount(session);
}

/// ver2
//void LanServer::completeRecv(Session* session, int numOfBytes)
//{
//	PRO(L"completeRecv 2");
//
//	session->recvPacket_->moveWritePos(numOfBytes);
//
//	if (session->recvPacket_->isFull())
//	{
//		LOG_INFO(L"recvQueue full");
//
//		//연결 종료 처리
//
//		return;
//	}
//
//	while (1)
//	{
//		int useSize = session->recvPacket_->useSize();
//
//		if (useSize < sizeof(HEADER))
//			break;
//
//		HEADER header;
//
//		session->recvPacket_->peek((char*)&header, sizeof(HEADER));
//
//		int messageSize = header.size_;
//
//		if (messageSize < 0)
//		{
//			// 연결 종료 처리
//
//			break;
//		}
//
//		if (useSize < sizeof(HEADER) + messageSize)
//			break;
//
//		session->recvPacket_->moveReadPos(sizeof(HEADER));
//
//		//Packet packet;
//		//session->recvQueue_.dequeue(packet.getBufferPtr(), messageSize);
//		//packet.moveWritePos(messageSize);
//
//
//		//printf("recvQueue dequeue : %lld\n", *(__int64*)packet.buffer());
//
//		this->onRecv(session->sessionId_, session->recvPacket_);
//
//		InterlockedIncrement(&recvMessageCount_);
//
//		if (session->recvPacket_->useSize() < sizeof(HEADER))
//			break;
//
//		RecvPacket* packet = new RecvPacket();
//		packet->initialize();
//		*packet = *session->recvPacket_;
//
//		decrementRecvPacketRefCount(session->recvPacket_);
//		session->recvPacket_ = packet;
//	}
//
//	this->recvPost(session);
//
//	this->decrementIoCount(session);
//}

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
//			LOG_INFO(L"recvQueue full");
//
//			//연결 종료 로직
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
//		Packet* packet = new Packet();
//
//		session->recvQueue_.dequeue(packet->getBufferPtr(), sizeof(unsigned char) + messageSize);
//
//		packet->moveWritePos(sizeof(unsigned char) + messageSize);
//
//		//printf("recvQueue dequeue : %lld\n", *(__int64*)packet.buffer());
//
//		this->onRecv(session->sessionId_, packet);
//
//		InterlockedIncrement(&recvMessageCount_);
//	}
//
//	this->recvPost(session);
//
//	this->decrementIoCount(session);
//}

/// MMOServer
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
//			LOG_INFO(L"recvQueue full");
//
//			//연결 종료 로직
//
//			return;
//		}
//
//		if (useSize < sizeof(MMO_HEADER))
//			break;
//
//		MMO_HEADER header;
//
//		session->recvQueue_.peek((char*)&header, sizeof(MMO_HEADER));
//
//		int messageSize = header.size;
//
//		if (messageSize < 0)
//			break;
//
//		if (useSize < sizeof(MMO_HEADER) + messageSize)
//			break;
//
//		session->recvQueue_.moveFront(sizeof(MMO_HEADER));
//
//		Packet* packet = new Packet();
//
//		session->recvQueue_.dequeue(packet->getBufferPtr(), messageSize);
//
//		packet->moveWritePos(messageSize);
//
//		//printf("recvQueue dequeue : %lld\n", *(__int64*)packet.buffer());
//		
//		this->onRecv(session->sessionId_, packet);
//
//		InterlockedIncrement(&recvMessageCount_);
//	}
//
//	this->recvPost(session);
//
//	this->decrementIoCount(session);
//}

void LanServer::sendPost(Session* session)
{
	if (InterlockedExchange(&session->sendPending_, 1) == 1)
	{
		return;
	}

	ZeroMemory(&session->sendOverlapped_.overlapped, sizeof(WSAOVERLAPPED));

	WSABUF wsaBuf[2];

	int retval;

	session->sendQueue_.lock();

	if (session->sendQueue_.useSize() <= session->sendQueue_.directDequeueSize())
	{
		// buf 1개
		wsaBuf[0].buf = session->sendQueue_.getFrontBufferPtr();
		wsaBuf[0].len = session->sendQueue_.useSize();

		session->sendQueue_.unlock();

		this->incrementIoCount(session);

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

		session->sendQueue_.unlock();

		this->incrementIoCount(session);

		//PRO_BEGIN(L"send 1");
		//PRO_BEGIN(L"send 2");
		retval = WSASend(session->socket_, wsaBuf, 2, nullptr, 0, (WSAOVERLAPPED*)&(session->sendOverlapped_), NULL);
		//PRO_END(L"send 1");
	}

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
	/// 락 필요???
	//session->sendQueue_.lock();

	session->sendQueue_.moveFront(numOfBytes);

	InterlockedExchange(&session->sendPending_, 0);

	if (session->sendQueue_.useSize() > 0)
		this->sendPost(session);

	//session->sendQueue_.unlock();

	this->decrementIoCount(session);
}


#define MAX_WSABUF 50

void LanServer::sendPostZeroCopy(Session* session)
{
	if (InterlockedExchange(&session->sendPending_, 1) == 1)
		return;

	ZeroMemory(&session->sendOverlapped_.overlapped, sizeof(WSAOVERLAPPED));

	session->sendQueueT_.lock();

	int n = session->sendQueueT_.useSize();

	// WsaSend() bufcount 인자 0이면 10022 에러 발생함. 예외처리
	if (n <= 0)
	{
		InterlockedExchange(&session->sendPending_, 0);

		session->sendQueueT_.unlock();

		return;
	}

	WSABUF wsaBuf[MAX_WSABUF];

	session->sendPacketCount_ = 0;

	for (int i = 0; i < n && i < MAX_WSABUF; i++)
	{
		SendPacket* packet = nullptr;

		session->sendQueueT_.peek(packet, i);

		wsaBuf[i].buf = packet->getBufferPtr();
		wsaBuf[i].len = packet->useSize();

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

	session->sendQueueT_.unlock();

	this->incrementIoCount(session);

	// wsaBuf count가 0이면 10022 에러 발생함
	int retval = WSASend(session->socket_, wsaBuf, session->sendPacketCount_, nullptr, 0, (WSAOVERLAPPED*)&(session->sendOverlapped_), NULL);

	if (retval == SOCKET_ERROR)
	{
		int error = WSAGetLastError();

		if (error == ERROR_IO_PENDING)
		{
			//printf("send Direct IO\n");
		}
		else if (error == WSAECONNRESET || error == WSAECONNABORTED || error == WSAENOTSOCK)
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

void LanServer::completeSendZeroCopy(Session* session, int numOfBytes)
{
	SendPacket* p = nullptr;

	session->sendQueueT_.lock();

	// numOfBytes는 신경 쓰지말고, sendPacketCount_로 처리
	for (int i = 0; i < session->sendPacketCount_; i++)
	{
		session->sendQueueT_.dequeue(p);

		/// delete p 추가
	}

	// sendPending 획득중에 큐의 useSize()를 확인하면
	// 여기서 0이어서 postSend()를 생략하는데,
	// 다른 스레드는 sendQ에 넣지만 pending을 획득 못해서 postSend()를 생략
	// 데이터가 있음에도 send를 못하는 문제 발생

	InterlockedExchange(&session->sendPending_, 0);

	if (session->sendQueueT_.useSize() > 0)
		this->sendPostZeroCopy(session);

	session->sendQueueT_.unlock();

	this->decrementIoCount(session);
}

void LanServer::incrementIoCount(Session* session)
{
	InterlockedIncrement(&session->ioCount_);
}

void LanServer::decrementIoCount(Session* session)
{
	if (InterlockedDecrement(&session->ioCount_) == 0)
	{
		this->releaseSession(session);

		return;
	}
}

void LanServer::releaseSession(Session* session)
{
	sessionMapLock_.lock();

	sessionMap_.erase(session->sessionId_);

	session->sessionLock_.lock();

	sessionMapLock_.unlock();

	session->sessionLock_.unlock();

	this->onRelease(session->sessionId_);

	delete session;

	//LOG_INFO(L"[NETWORK] session delete");

	InterlockedDecrement(&sessionCount_);
}
