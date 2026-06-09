#pragma once
#include "../Utils/Singleton.h"

class Session;
class Packet;

class IPacketHandler
{
public:
	virtual ~IPacketHandler() = 0;

	virtual bool sc_create_my_character(Session* session, int id, char direction, short x, short y, char hp);
	virtual bool sc_create_other_character(Session* session, int id, char direction, short x, short y, char hp);
	virtual bool sc_character_delete(Session* session, int id);
	virtual bool cs_start_move(Session* session, char direction, short x, short y);
	virtual bool sc_start_move(Session* session, int id, char direction, short x, short y);
	virtual bool cs_stop_move(Session* session, char action, short x, short y);
	virtual bool sc_stop_move(Session* session, int id, char action, short x, short y);
	virtual bool cs_attack1(Session* session, char direction, short x, short y);
	virtual bool sc_attack1(Session* session, int id, char direction, short x, short y);
	virtual bool cs_attack2(Session* session, char direction, short x, short y);
	virtual bool sc_attack2(Session* session, int id, char direction, short x, short y);
	virtual bool cs_attack3(Session* session, char direction, short x, short y);
	virtual bool sc_attack3(Session* session, int id, char direction, short x, short y);
	virtual bool sc_damage(Session* session, int attackID, int damageID, char damageHP);
};

class RPCStub : public Singleton<RPCStub>
{
private:
	IPacketHandler* handle_ = nullptr;

public:
	void initialize(IPacketHandler* handle);
	bool packetProc(Session* session, Packet& packet, int type);
};
