#include "Session.h"	
#include "SessionManager.h"
#include "../Utils/Packet.h"
#include "../Utils/Logger.h"

LONG cnt = 0;

void Session::initialize(SOCKET socket, std::wstring ip, int port, __int64 id)
{
	lock_.lock();

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

	lock_.unlock();

	//LOG_INFO(L"[NETWORK] session ip=%s port=%d", ip_.c_str(), port_);
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
	lock_.lock();

	// 처음에 recv를 등록하기전에 카운트를 증가시킴!!!
	InterlockedIncrement(&ioCount_);

	ZeroMemory(&recvOverlapped_.overlapped, sizeof(WSAOVERLAPPED));

	lock_.unlock();

	DWORD flags = 0;
	int retval1;

	WSABUF wsaBuf[2];

	lock_.lock();

	wsaBuf[0].buf = recvQueue_.getRearBufferPtr();
	wsaBuf[0].len = recvQueue_.directEnqueueSize();

	if (recvQueue_.freeSize() == recvQueue_.directEnqueueSize())
	{
		// buf 1개
		retval1 = WSARecv(socket_, wsaBuf, 1, nullptr, &flags, (WSAOVERLAPPED*)&recvOverlapped_, NULL);
	}
	else
	{
		// buf 2개
		wsaBuf[1].buf = recvQueue_.getBufferPtr();
		wsaBuf[1].len = recvQueue_.freeSize() - recvQueue_.directEnqueueSize();

		retval1 = WSARecv(socket_, wsaBuf, 2, nullptr, &flags, (WSAOVERLAPPED*)&recvOverlapped_, NULL);
	}

	lock_.unlock();

	if (retval1 == SOCKET_ERROR)
	{
		int error = WSAGetLastError();

		if (error == ERROR_IO_PENDING)
		{
			//printf("recv() Direct IO\n");
		}
		else if (error == WSAECONNRESET)
		{
			this->decrementIOCount();

			return;
		}
		else
		{
			LOG(L"WSARecv() error: %d, iocount: %d", error, ioCount_);

			this->decrementIOCount();

			return;
		}
	}
	else
	{
		//printf("recv() Fast IO\n");
	}
}

// 현재 전역함수
void onRecv(__int64 sessionId, Packet& packet)
{
	MESSAGE message;
	packet >> message.data_;

	Packet packet2;
	packet2 << message.data_;

	SessionManager::getInstance().sendPacket(sessionId, packet2);
}

void Session::completeRecv(int numOfBytes)
{
	//printf("completeRecv() %d\n", numOfBytes);
	lock_.lock();

	recvQueue_.moveRear(numOfBytes);

	lock_.unlock();

	while (1)
	{
		lock_.lock();

		int useSize = recvQueue_.useSize();

		if (recvQueue_.isFull())
		{
			LOG_INFO(L"recvQueue full");

			//연결 종료 로직 필요
		}

		lock_.unlock();

		if (useSize < sizeof(HEADER))
			break;

		HEADER header;

		lock_.lock();

		recvQueue_.peek((char*)&header, sizeof(HEADER));

		lock_.unlock();

		int headerSize = header.size;

		if (useSize < sizeof(HEADER) + headerSize)
			break;

		lock_.lock();

		recvQueue_.moveFront(sizeof(HEADER));

		Packet packet;

		recvQueue_.dequeue(packet.getBufferPtr(), headerSize);

		lock_.unlock();

		packet.moveWritePos(headerSize);

		//printf("recvQueue dequeue : %lld\n", *(__int64*)packet.buffer());

		onRecv(sessionId_, packet);
	}

	this->postRecv();

	this->decrementIOCount();
}

void Session::postSend()
{
	lock_.lock();

	if (InterlockedExchange(&sendPending_, 1) == 1)
	{
		lock_.unlock();

		return;
	}

	// WSASend() 이전에 호출해야, 
	// 다른 스레드에서 send 완료통지로 ioCount 내려서 0이 되는 상황 차단
	InterlockedIncrement(&ioCount_);

	ZeroMemory(&sendOverlapped_.overlapped, sizeof(WSAOVERLAPPED));

	lock_.unlock();

	int retval;

	WSABUF wsaBuf[2];

	lock_.lock();

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

	lock_.unlock();

	if (retval == SOCKET_ERROR)
	{
		int error = WSAGetLastError();

		if (error == ERROR_IO_PENDING)
		{
			//printf("send Direct IO\n");
		}
		else if (error == WSAECONNRESET)
		{
			this->decrementIOCount();

			return;
		}
		else
		{
			LOG(L"WSASend() error: %d, iocount: %d", error, ioCount_);

			this->decrementIOCount();

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
	lock_.lock();

	sendQueue_.moveFront(numOfBytes);

	sendPending_ = 0;

	// 추가
	int useSize = sendQueue_.useSize();

	lock_.unlock();

	if (useSize > 0)
		this->postSend();

	//printf("sendq usesize: %d\n", useSize);

	this->decrementIOCount();
}

void Session::sendPacket(Packet& packet)
{
	//if (ioCount_ == 0)
	//	return;

	//if(InterlockedIncrement(&ioCount_) == 1)
	//	return;

	HEADER header;
	header.size = packet.useSize();

	/// 여길 반드시 잠궈야 oversend 정상적으로 동작
	lock_.lock();

	sendQueue_.enqueue((char*)&header, sizeof(HEADER));
	sendQueue_.enqueue(packet.getBufferPtr(), packet.useSize());

	lock_.unlock();

	this->postSend();
}

void Session::decrementIOCount()
{
	lock_.lock();

	if (InterlockedDecrement(&ioCount_) == 0)
	{
		SessionManager::getInstance().removeSession(this);
	}

	lock_.unlock();
}
