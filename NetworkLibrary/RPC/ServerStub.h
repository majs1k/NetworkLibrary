#pragma once

class LanServer;
class Packet;

class ServerStub
{
public:
	bool packetProc(__int64 sessionId, Packet& packet, unsigned char type);

	virtual bool sc_create_my_character(__int64 sessionId, int id, char direction, short x, short y, char hp);
	virtual bool sc_create_other_character(__int64 sessionId, int id, char direction, short x, short y, char hp);
	virtual bool sc_character_delete(__int64 sessionId, int id);
	virtual bool cs_start_move(__int64 sessionId, char direction, short x, short y);
	virtual bool sc_start_move(__int64 sessionId, int id, char direction, short x, short y);
	virtual bool cs_stop_move(__int64 sessionId, char action, short x, short y);
	virtual bool sc_stop_move(__int64 sessionId, int id, char action, short x, short y);
	virtual bool cs_attack1(__int64 sessionId, char direction, short x, short y);
	virtual bool sc_attack1(__int64 sessionId, int id, char direction, short x, short y);
	virtual bool cs_attack2(__int64 sessionId, char direction, short x, short y);
	virtual bool sc_attack2(__int64 sessionId, int id, char direction, short x, short y);
	virtual bool cs_attack3(__int64 sessionId, char direction, short x, short y);
	virtual bool sc_attack3(__int64 sessionId, int id, char direction, short x, short y);
	virtual bool sc_damage(__int64 sessionId, int attackID, int damageID, char damageHP);
};
