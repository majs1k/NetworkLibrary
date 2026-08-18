#include "ServerProxy.h"
#include "../Network/LanServer.h"
#include "../Utils/Packet.h"

void ServerProxy::cs_create_login(__int64 sessionId, int num)
{
	Packet* packet = new Packet();
	packet->Initialize();

	packet->GetHeaderPtr()->type_ = 0;
	*packet << num;

	server_->SendPacket(sessionId, packet);
}

void ServerProxy::sc_create_login(__int64 sessionId, int num)
{
	Packet* packet = new Packet();
	packet->Initialize();

	packet->GetHeaderPtr()->type_ = 1;
	*packet << num;

	server_->SendPacket(sessionId, packet);
}
