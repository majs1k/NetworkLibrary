#include "TestServer.h"
#include "../Utils/Packet.h"
#include "../Utils/PacketT.h"
#include "../Utils/Buffer.h"
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
//void TestServer::onRecv(__int64 sessionId, Packet& packet)
//{
//	MESSAGE message;
//	packet >> message.data_;
//
//	Packet packet2;
//	packet2 << message.data_;
//
//	// 호출부 안에서 패킷 헤더를 삽입
//	this->sendPacket(sessionId, packet2);
//}

/// ver2
void TestServer::onRecv(__int64 sessionId, Packet& packet)
{
	MESSAGE message;
	packet >> message.data_;

	//printf("%lld\n", message.data_);

	//Packet packet2;
	//packet2 << message.data_;

	Buffer* buffer = new Buffer();
	*buffer << message.data_;

	buffer->increase(1);

	/// 여러 세션으로 send 가능
	this->sendPacket(sessionId, buffer);
}

void TestServer::onError(int errorCode, wchar_t* str)
{

}
