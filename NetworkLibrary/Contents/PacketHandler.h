#pragma once
#include "../RPC/RPCStub.h"

class Session;

class PacketHandler : public IPacketHandler
{
public:
	bool cs_start_move(Session* session, char action, short x, short y) override;
	bool cs_stop_move(Session* session, char direction, short x, short y) override;
	bool cs_attack1(Session* session, char direction, short x, short y) override;
};
