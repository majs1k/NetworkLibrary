#pragma once
#include "../RPC/RPCStub.h"

class PacketHandler : public IPacketHandler
{
public:
	bool cs_start_move(__int64 sessionId, char action, short x, short y) override;
	bool cs_stop_move(__int64 sessionId, char direction, short x, short y) override;
	bool cs_attack1(__int64 sessionId, char direction, short x, short y) override;
};
