#pragma once

class Session;

class RPCProxy
{
public:
	static void sc_create_my_character(Session* session, int id, char direction, short x, short y, char hp);
	static void sc_create_other_character(Session* session, int id, char direction, short x, short y, char hp);
	static void sc_character_delete(Session* session, int id);
	static void cs_start_move(Session* session, char direction, short x, short y);
	static void sc_start_move(Session* session, int id, char direction, short x, short y);
	static void cs_stop_move(Session* session, char action, short x, short y);
	static void sc_stop_move(Session* session, int id, char action, short x, short y);
	static void cs_attack1(Session* session, char direction, short x, short y);
	static void sc_attack1(Session* session, int id, char direction, short x, short y);
	static void cs_attack2(Session* session, char direction, short x, short y);
	static void sc_attack2(Session* session, int id, char direction, short x, short y);
	static void cs_attack3(Session* session, char direction, short x, short y);
	static void sc_attack3(Session* session, int id, char direction, short x, short y);
	static void sc_damage(Session* session, int attackID, int damageID, char damageHP);
};
