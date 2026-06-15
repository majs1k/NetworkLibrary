#include "TestServer.h"
#include "../Utils/Packet.h"
#include "../Utils/Logger.h"

bool TestServer::onConnectionRequest(const std::wstring& ip, int port)
{
	if (sessionCount_ >= sessionMax_)
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

void TestServer::onRecv(__int64 sessionId, Packet& packet)
{
	MESSAGE message;
	packet >> message.data_;

	Packet packet2;
	packet2 << message.data_;

	// 호출부 안에서 패킷 헤더를 삽입
	this->sendPacket(sessionId, packet2);
}

void TestServer::onError(int errorCode, wchar_t* str)
{

}
