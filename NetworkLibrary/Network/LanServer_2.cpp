#include "LanServer.h"
#include "Session.h"
#include "../Utils/Packet.h"
#include "../Utils/Logger.h"
#include "../Utils/Profiler.h"
#include <Windows.h>

/// recvQ의 경우는 한스레드에서만 접근하는것이 보장되므로 락 사용 x
//void LanServer::RecvPost(Session* session)
//{
//	ZeroMemory(&session->recvOverlapped_.overlapped, sizeof(WSAOVERLAPPED));
//
//	WSABUF wsaBuf[2];
//
//	wsaBuf[0].buf = session->recvQueue_->GetRearBufferPtr();
//	wsaBuf[0].len = session->recvQueue_->DirectEnqueueSize();
//
//	DWORD flags = 0;
//	int retval;
//
//	if (session->recvQueue_->FreeSize() == session->recvQueue_->DirectEnqueueSize())
//	{
//		// WSARecv() 이전에 호출
//		this->IncrementIoCount(session);
//
//		// buf 1개
//		retval = WSARecv(session->socket_, wsaBuf, 1, nullptr, &flags, (WSAOVERLAPPED*)&session->recvOverlapped_, NULL);
//	}
//	else
//	{
//		this->IncrementIoCount(session);
//
//		// buf 2개
//		wsaBuf[1].buf = session->recvQueue_->GetBufferPtr();
//		wsaBuf[1].len = session->recvQueue_->FreeSize() - session->recvQueue_->DirectEnqueueSize();
//
//		retval = WSARecv(session->socket_, wsaBuf, 2, nullptr, &flags, (WSAOVERLAPPED*)&session->recvOverlapped_, NULL);
//	}
//
//	if (retval == SOCKET_ERROR)
//	{
//		int error = WSAGetLastError();
//
//		if (error == ERROR_IO_PENDING)
//		{
//			//printf("recv() Direct IO\n");
//		}
//		else if (error == WSAECONNRESET || error == WSAECONNABORTED || error == WSAENOTSOCK)
//		{
//			this->DecrementIoCount(session);
//
//			return;
//		}
//		else
//		{
//			LOG(L"WSARecv() error: %d, iocount: %d", error, session->ioCount_);
//			__debugbreak();
//			this->DecrementIoCount(session);
//
//			return;
//		}
//	}
//	else
//	{
//		//printf("recv() Fast IO\n");
//	}
//}

void LanServer::RecvPost(Session* session)
{
	PRO(L"RecvPost()");

	ZeroMemory(&session->recvOverlapped_.overlapped, sizeof(WSAOVERLAPPED));

	WSABUF wsaBuf;

	wsaBuf.buf = session->recvQueue_->GetBufferPtr();
	wsaBuf.len = session->recvQueue_->FreeSize();

	DWORD flags = 0;
	int retval;

	// WSARecv() 이전에 호출
	this->IncrementIoCount(session);

	retval = WSARecv(session->socket_, &wsaBuf, 1, nullptr, &flags, (WSAOVERLAPPED*)&session->recvOverlapped_, NULL);

	if (retval == SOCKET_ERROR)
	{
		int error = WSAGetLastError();

		if (error == ERROR_IO_PENDING)
		{
			//printf("recv() Direct IO\n");
		}
		else if (error == WSAECONNRESET || error == WSAECONNABORTED)
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

void LanServer::CompleteRecv(Session* session, int numOfBytes)
{
	PRO(L"CompleteRecv()");

	session->recvQueue_->MoveRear(numOfBytes);

	while (1)
	{
		int useSize = session->recvQueue_->UseSize();

		if (session->recvQueue_->IsFull())
		{
			LOG_INFO(L"recvQueue full");

			__debugbreak();

			// 연결 종료 로직

			return;
		}

		///TestServer
		if (useSize < sizeof(TEST_HEADER))
			break;

		TEST_HEADER header;

		session->recvQueue_->Peek((char*)&header, sizeof(TEST_HEADER));

		int messageSize = header.size_;

		if (messageSize < 0)
			break;

		if (useSize < sizeof(TEST_HEADER) + messageSize)
			break;

		session->recvQueue_->MoveFront(sizeof(TEST_HEADER));


		RPacket* packet = new RPacket();

		packet->Initialize(session->recvQueue_);
		session->recvQueue_->MoveFront(messageSize);

		packet->MoveWritePos(messageSize);




		///FighterServer
		//if (useSize < sizeof(FIGHTER_HEADER))
		//	break;
		//
		//FIGHTER_HEADER header;
		//
		//session->recvQueue_->Peek((char*)&header, sizeof(FIGHTER_HEADER));
		//
		//int messageSize = header.size;
		//
		//if (messageSize < 0)
		//	break;
		//
		//if (useSize < sizeof(FIGHTER_HEADER) + sizeof(unsigned char) + messageSize)
		//	break;
		//
		//session->recvQueue_->MoveFront(sizeof(FIGHTER_HEADER));
		//
		//RPacket* packet = new RPacket();
		//
		//packet->Initialize(session->recvQueue_);
		//session->recvQueue_->MoveFront(sizeof(unsigned char) +  messageSize);
		//
		//packet->MoveWritePos(sizeof(unsigned char) + messageSize);





		this->OnRecv(session->sessionId_, packet);

		InterlockedIncrement(&recvMessageCount_);
	}

	//RingBuffer* buffer = new RingBuffer(RECV_SIZE);
	auto buffer = std::make_shared<RingBuffer>(RECV_SIZE);

	if (session->recvQueue_->UseSize() > 0)
	{
		buffer->Enqueue(session->recvQueue_->GetFrontBufferPtr(), session->recvQueue_->UseSize());
	}

	//delete session->recvQueue_;

	session->recvQueue_ = buffer;

	this->RecvPost(session);

	this->DecrementIoCount(session);
}

/// Send 복사 생략 이전 버전
//void LanServer::SendPost(Session* session)
//{
//	//PRO(L"SendPost");
//
//	if (InterlockedExchange(&session->sendPending_, 1) == 1)
//	{
//		return;
//	}
//
//	ZeroMemory(&session->sendOverlapped_.overlapped, sizeof(WSAOVERLAPPED));
//
//	WSABUF wsaBuf[2] = {};
//
//	int retval;
//
//	session->sessionLock_.lock();
//
//	if (session->sendQueue_.UseSize() <= session->sendQueue_.DirectDequeueSize())
//	{
//		// buf 1개
//		wsaBuf[0].buf = session->sendQueue_.GetFrontBufferPtr();
//		wsaBuf[0].len = session->sendQueue_.UseSize();
//
//		session->sessionLock_.unlock();
//
//		this->IncrementIoCount(session);
//
//		retval = WSASend(session->socket_, wsaBuf, 1, nullptr, 0, (WSAOVERLAPPED*)&(session->sendOverlapped_), NULL);
//	}
//	else
//	{
//		// buf 2개
//		wsaBuf[0].buf = session->sendQueue_.GetFrontBufferPtr();
//		wsaBuf[0].len = session->sendQueue_.DirectDequeueSize();
//
//		wsaBuf[1].buf = session->sendQueue_.GetBufferPtr();
//		wsaBuf[1].len = session->sendQueue_.UseSize() - session->sendQueue_.DirectDequeueSize();
//
//		session->sessionLock_.unlock();
//
//		this->IncrementIoCount(session);
//
//		retval = WSASend(session->socket_, wsaBuf, 2, nullptr, 0, (WSAOVERLAPPED*)&(session->sendOverlapped_), NULL);
//	}
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
//			if (session->ioCount_ == 1)
//				__debugbreak();
//
//			this->DecrementIoCount(session);
//
//			return;
//		}
//		else
//		{
//			LOG(L"WSASend() error: %d, iocount: %d", error, session->ioCount_);
//			__debugbreak();
//
//			this->DecrementIoCount(session);
//
//			return;
//		}
//	}
//	else
//	{
//		//printf("send Fast IO\n");
//	}
//}
//
//void LanServer::CompleteSend(Session* session, int numOfBytes)
//{
//	//PRO(L"CompleteSend");
//
//	session->sessionLock_.lock();
//
//	session->sendQueue_.MoveFront(numOfBytes);
//
//	InterlockedExchange(&session->sendPending_, 0);
//
//	int s = session->sendQueue_.UseSize();
//
//	session->sessionLock_.unlock();
//
//	if (s > 0)
//		this->SendPost(session);
//
//	this->DecrementIoCount(session);
//}


#define MAX_WSABUF 100

void LanServer::SendPost(Session* session)
{
	PRO(L"SendPost()");

	if (InterlockedExchange(&session->sendPending_, 1) == 1)
		return;

	ZeroMemory(&session->sendOverlapped_.overlapped, sizeof(WSAOVERLAPPED));
	session->sendPacketCount_ = 0;

	session->sessionLock_.lock();

	int n = min(session->sendQueue_.UseSize(), MAX_WSABUF);

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
		Packet* packet = session->sendQueue_.At(i);

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

void LanServer::CompleteSend(Session* session, int numOfBytes)
{
	PRO(L"CompleteSend()");

	Packet* p = nullptr;

	session->sessionLock_.lock();

	// numOfBytes는 신경 쓰지말고, sendPacketCount_로 처리
	for (int i = 0; i < session->sendPacketCount_; i++)
	{
		/// 참조카운트로 수정시 p 필요없음
		session->sendQueue_.Dequeue(p);

		delete p;
	}

	InterlockedExchange(&session->sendPending_, 0);

	int s = session->sendQueue_.UseSize();

	session->sessionLock_.unlock();

	if (s > 0)
		this->SendPost(session);

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
