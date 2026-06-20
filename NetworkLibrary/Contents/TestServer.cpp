#include "TestServer.h"
#include "../Utils/Packet.h"
#include "../Utils/SendPacket.h"
#include "../Utils/RingBufferT.h"
#include "../Utils/Logger.h"

bool TestServer::onConnectionRequest(const std::wstring& ip, int port)
{
	if (sessionCount() >= sessionMax())
	{
		LOG(L"[Network] session limit over");

		return false;
	}

	return true;
}

void TestServer::onAccept(__int64 sessionId)
{

}

void TestServer::onRelease(__int64 sessionId)
{

}

/// ver1
void TestServer::onRecv(__int64 sessionId, Packet& packet)
{
	MESSAGE message;
	packet >> message.data_;

	Packet packet2;
	packet2 << message.data_;

	// 호출부 안에서 패킷 헤더를 삽입
	this->sendPacket(sessionId, &packet2);
}

/// ver2
//void TestServer::onRecv(__int64 sessionId, Packet& packet)
//{
//	MESSAGE message;
//	packet >> message.data_;
//
//	//printf("%lld\n", message.data_);
//
//	//Packet packet2;
//	//packet2 << message.data_;
//
//	SendPacket* p = new SendPacket();
//	this->incrementPacketRefCount(p);
//
//	*p << message.data_;
//
//	/// 여러 세션으로 send 가능
//	this->sendPacketSkipCopy(sessionId, p);
//
//	this->decrementPacketRefCount(p);
//}

void TestServer::onError(int errorCode, wchar_t* str)
{

}
