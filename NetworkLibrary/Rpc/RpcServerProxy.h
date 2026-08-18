#pragma once

#include "../Network/LanServer.h"
#include "../Utils/Packet.h"

class RpcServerProxy
{
public:
	LanServer* server_;

public:
	void LoginRequest(__int64 sessionId, int id)
	{
		Packet* packet = new Packet();
		packet->Initialize();

		packet->GetHeaderPtr()->type_ = 0;
		*packet << id;

		server_->SendPacket(sessionId, packet);
	}
	void LoginResponse(__int64 sessionId, int id)
	{
		Packet* packet = new Packet();
		packet->Initialize();

		packet->GetHeaderPtr()->type_ = 1;
		*packet << id;

		server_->SendPacket(sessionId, packet);
	}
};
