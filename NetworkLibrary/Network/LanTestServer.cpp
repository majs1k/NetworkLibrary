#include "LanTestServer.h"
#include "../Utils/Packet.h"

void LanTestServer::onAccept(__int64 sessionId)
{

}

void LanTestServer::onRelease(__int64 sessionId)
{

}

void LanTestServer::onRecv(__int64 sessionId, Packet& packet)
{
	MESSAGE message;
	packet >> message.data_;

	Packet packet2;
	packet2 << message.data_;

	// 호출부 안에서 패킷 헤더를 삽입
	this->sendPacket(sessionId, packet2);
}

void LanTestServer::onError(int errorCode, wchar_t* str)
{

}
