#pragma once

class LanServer;
class Packet;

class IRpcStub
{
public:
	bool PacketProc(__int64 sessionId, Packet* packet);

	virtual bool cs_create_login(__int64 sessionId, int num);
	virtual bool sc_create_login(__int64 sessionId, int num);
};
