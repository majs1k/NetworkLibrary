#pragma once

class LanServer;

class RPCProxy
{
public:
	LanServer* server;

public:
	void sc_create_my_character(__int64 sessionId, int id, char direction, short x, short y, char hp);
	void sc_create_other_character(__int64 sessionId, int id, char direction, short x, short y, char hp);
	void sc_character_delete(__int64 sessionId, int id);
	void cs_start_move(__int64 sessionId, char direction, short x, short y);
	void sc_start_move(__int64 sessionId, int id, char direction, short x, short y);
	void cs_stop_move(__int64 sessionId, char action, short x, short y);
	void sc_stop_move(__int64 sessionId, int id, char action, short x, short y);
	void cs_attack1(__int64 sessionId, char direction, short x, short y);
	void sc_attack1(__int64 sessionId, int id, char direction, short x, short y);
	void cs_attack2(__int64 sessionId, char direction, short x, short y);
	void sc_attack2(__int64 sessionId, int id, char direction, short x, short y);
	void cs_attack3(__int64 sessionId, char direction, short x, short y);
	void sc_attack3(__int64 sessionId, int id, char direction, short x, short y);
	void sc_damage(__int64 sessionId, int attackID, int damageID, char damageHP);
};
