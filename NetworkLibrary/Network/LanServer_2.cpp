#include "LanServer.h"
#include "Session.h"
#include "../Utils/Packet.h"
#include "../Utils/RecvPacket.h"
#include "../Utils/Logger.h"
#include "../Utils/Profiler.h"
#include <Windows.h>

/// ver1
void LanServer::RecvPost(Session* session)
{
	//PRO(L"postRecv 1");

	ZeroMemory(&session->recvOverlapped_.overlapped, sizeof(WSAOVERLAPPED));

	WSABUF wsaBuf[2];

	wsaBuf[0].buf = session->recvQueue_.GetRearBufferPtr();
	wsaBuf[0].len = session->recvQueue_.DirectEnqueueSize();

	DWORD flags = 0;
	int retval;

	if (session->recvQueue_.FreeSize() == session->recvQueue_.DirectEnqueueSize())
	{
		// WSARecv() 이전에 호출
		this->IncrementIoCount(session);

		// buf 1개
		retval = WSARecv(session->socket_, wsaBuf, 1, nullptr, &flags, (WSAOVERLAPPED*)&session->recvOverlapped_, NULL);
	}
	else
	{
		this->IncrementIoCount(session);

		// buf 2개
		wsaBuf[1].buf = session->recvQueue_.GetBufferPtr();
		wsaBuf[1].len = session->recvQueue_.FreeSize() - session->recvQueue_.DirectEnqueueSize();

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
			this->DecrementIoCount(session);

			return;
		}
		else
		{
			LOG(L"WSARecv() error: %d, iocount: %d", error, session->ioCount_);
			__debugbreak();
			this->DecrementIoCount(session);

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
//			__debugbreak();
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
void LanServer::CompleteRecv(Session* session, int numOfBytes)
{
	//PRO(L"completeRecv 1");

	/// recvQ의 경우는 한스레드에서만 접근하는것이 보장되므로 락 사용 x

	session->recvQueue_.MoveRear(numOfBytes);

	while (1)
	{
		int useSize = session->recvQueue_.UseSize();

		if (session->recvQueue_.IsFull())
		{
			LOG_INFO(L"recvQueue full");

			__debugbreak();

			// 연결 종료 로직

			return;
		}

		if (useSize < sizeof(TEST_HEADER))
			break;

		TEST_HEADER header;

		session->recvQueue_.Peek((char*)&header, sizeof(TEST_HEADER));

		int messageSize = header.size_;

		if (messageSize < 0)
			break;

		if (useSize < sizeof(TEST_HEADER) + messageSize)
			break;

		session->recvQueue_.MoveFront(sizeof(TEST_HEADER));

		Packet* packet = new Packet();

		session->recvQueue_.Dequeue(packet->GetBodyPtr(), messageSize);

		packet->MoveWritePos(messageSize);

		//printf("recvQueue dequeue : %lld\n", *(__int64*)packet.buffer());

		this->OnRecv(session->sessionId_, packet);

		InterlockedIncrement(&recvMessageCount_);
	}

	this->RecvPost(session);

	this->DecrementIoCount(session);
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
//void LanServer::CompleteRecv(Session* session, int numOfBytes)
//{
//	session->recvQueue_.MoveRear(numOfBytes);
//
//	while (1)
//	{
//		int useSize = session->recvQueue_.UseSize();
//
//		if (session->recvQueue_.IsFull())
//		{
//			LOG_INFO(L"recvQueue full");
//
//			__debugbreak();
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
//		session->recvQueue_.Peek((char*)&header, sizeof(FIGHTER_HEADER));
//
//		if (header.code != PACKET_CODE)
//		{
//			this->DecrementIoCount(session);
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
//		session->recvQueue_.MoveFront(sizeof(FIGHTER_HEADER));
//
//		Packet* packet = new Packet();
//
//		session->recvQueue_.Dequeue(packet->GetBufferPtr(), sizeof(unsigned char) + messageSize);
//
//		packet->MoveWritePos(sizeof(unsigned char) + messageSize);
//
//		//printf("recvQueue dequeue : %lld\n", *(__int64*)packet.buffer());
//
//		this->OnRecv(session->sessionId_, packet);
//
//		InterlockedIncrement(&recvMessageCount_);
//	}
//
//	this->RecvPost(session);
//
//	this->DecrementIoCount(session);
//}

void LanServer::SendPost(Session* session)
{
	//PRO(L"SendPost");

	if (InterlockedExchange(&session->sendPending_, 1) == 1)
	{
		return;
	}

	ZeroMemory(&session->sendOverlapped_.overlapped, sizeof(WSAOVERLAPPED));

	WSABUF wsaBuf[2] = {};

	int retval;

	session->sessionLock_.lock();

	if (session->sendQueue_.UseSize() <= session->sendQueue_.DirectDequeueSize())
	{
		// buf 1개
		wsaBuf[0].buf = session->sendQueue_.GetFrontBufferPtr();
		wsaBuf[0].len = session->sendQueue_.UseSize();

		session->sessionLock_.unlock();

		this->IncrementIoCount(session);

		retval = WSASend(session->socket_, wsaBuf, 1, nullptr, 0, (WSAOVERLAPPED*)&(session->sendOverlapped_), NULL);
	}
	else
	{
		// buf 2개
		wsaBuf[0].buf = session->sendQueue_.GetFrontBufferPtr();
		wsaBuf[0].len = session->sendQueue_.DirectDequeueSize();

		wsaBuf[1].buf = session->sendQueue_.GetBufferPtr();
		wsaBuf[1].len = session->sendQueue_.UseSize() - session->sendQueue_.DirectDequeueSize();

		session->sessionLock_.unlock();

		this->IncrementIoCount(session);

		retval = WSASend(session->socket_, wsaBuf, 2, nullptr, 0, (WSAOVERLAPPED*)&(session->sendOverlapped_), NULL);
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
			if (session->ioCount_ == 1)
				__debugbreak();

			this->DecrementIoCount(session);

			return;
		}
		else
		{
			LOG(L"WSASend() error: %d, iocount: %d", error, session->ioCount_);
			__debugbreak();

			this->DecrementIoCount(session);

			return;
		}
	}
	else
	{
		//printf("send Fast IO\n");
	}
}

void LanServer::CompleteSend(Session* session, int numOfBytes)
{
	//PRO(L"CompleteSend");

	session->sessionLock_.lock();

	session->sendQueue_.MoveFront(numOfBytes);

	InterlockedExchange(&session->sendPending_, 0);

	int s = session->sendQueue_.UseSize();

	session->sessionLock_.unlock();

	if (s > 0)
		this->SendPost(session);

	this->DecrementIoCount(session);
}


#define MAX_WSABUF 100

void LanServer::SendPostZeroCopy(Session* session)
{
	if (InterlockedExchange(&session->sendPending_, 1) == 1)
		return;

	ZeroMemory(&session->sendOverlapped_.overlapped, sizeof(WSAOVERLAPPED));
	session->sendPacketCount_ = 0;

	session->sessionLock_.lock();

	int n = min(session->sendQueue2_.UseSize(), MAX_WSABUF);

	// WSASend() bufcount 인자 0이면 10022 에러 발생함. 예외처리
	if (n <= 0)
	{
		InterlockedExchange(&session->sendPending_, 0);

		session->sessionLock_.unlock();

		return;
	}

	WSABUF wsaBuf[MAX_WSABUF];

	for (int i = 0; i < n; i++)
	{
		Packet* packet = nullptr;

		session->sendQueue2_.Peek(packet, i);

		wsaBuf[i].buf = packet->GetHeaderPtr();
		wsaBuf[i].len = packet->TotalUseSize();

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

	session->sessionLock_.unlock();

	this->IncrementIoCount(session);

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
			this->DecrementIoCount(session);

			return;
		}
		else
		{
			LOG(L"WSASend() error: %d", error);
			__debugbreak();

			this->DecrementIoCount(session);
			return;
		}
	}
	else
	{
		//printf("send Fast IO\n");
	}
}

void LanServer::CompleteSendZeroCopy(Session* session, int numOfBytes)
{
	Packet* p = nullptr;

	session->sendQueue2_.Lock();

	// numOfBytes는 신경 쓰지말고, sendPacketCount_로 처리
	for (int i = 0; i < session->sendPacketCount_; i++)
	{
		session->sendQueue2_.Dequeue(p);

		delete p;
	}

	InterlockedExchange(&session->sendPending_, 0);

	if (session->sendQueue2_.UseSize() > 0)
		this->SendPostZeroCopy(session);

	session->sendQueue2_.Unlock();

	this->DecrementIoCount(session);
}

void LanServer::IncrementIoCount(Session* session)
{
	InterlockedIncrement(&session->ioCount_);
}

void LanServer::DecrementIoCount(Session* session)
{
	sessionMapLock_.lock();

	session->sessionLock_.lock();

	if (InterlockedDecrement(&session->ioCount_) == 0)
	{
		sessionMap_.erase(session->sessionId_);

		sessionMapLock_.unlock();

		session->sessionLock_.unlock();

		this->OnRelease(session->sessionId_);

		delete session;

		//LOG_INFO(L"[NETWORK] session delete");

		InterlockedDecrement(&sessionCount_);
	}
	else
	{
		sessionMapLock_.unlock();

		session->sessionLock_.unlock();
	}
}

//void LanServer::ReleaseSession(Session* session)
//{
//	sessionMapLock_.lock();
//
//	sessionMap_.erase(session->sessionId_);
//
//	session->sessionLock_.lock();
//
//	sessionMapLock_.unlock();
//
//	session->sessionLock_.unlock();
//
//	this->OnRelease(session->sessionId_);
//
//	delete session;
//
//	//LOG_INFO(L"[NETWORK] session delete");
//
//	InterlockedDecrement(&sessionCount_);
//}
