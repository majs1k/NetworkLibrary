#include "RPCStub.h"

IPacketHandler::~IPacketHandler()
{

}

bool IPacketHandler::sc_create_my_character(Session* session, int id, char direction, short x, short y, char hp)
{
	return true;
}

bool IPacketHandler::sc_create_other_character(Session* session, int id, char direction, short x, short y, char hp)
{
	return true;
}

bool IPacketHandler::sc_character_delete(Session* session, int id)
{
	return true;
}

bool IPacketHandler::cs_start_move(Session* session, char direction, short x, short y)
{
	return true;
}

bool IPacketHandler::sc_start_move(Session* session, int id, char direction, short x, short y)
{
	return true;
}

bool IPacketHandler::cs_stop_move(Session* session, char action, short x, short y)
{
	return true;
}

bool IPacketHandler::sc_stop_move(Session* session, int id, char action, short x, short y)
{
	return true;
}

bool IPacketHandler::cs_attack1(Session* session, char direction, short x, short y)
{
	return true;
}

bool IPacketHandler::sc_attack1(Session* session, int id, char direction, short x, short y)
{
	return true;
}

bool IPacketHandler::cs_attack2(Session* session, char direction, short x, short y)
{
	return true;
}

bool IPacketHandler::sc_attack2(Session* session, int id, char direction, short x, short y)
{
	return true;
}

bool IPacketHandler::cs_attack3(Session* session, char direction, short x, short y)
{
	return true;
}

bool IPacketHandler::sc_attack3(Session* session, int id, char direction, short x, short y)
{
	return true;
}

bool IPacketHandler::sc_damage(Session* session, int attackID, int damageID, char damageHP)
{
	return true;
}

void RPCStub::initialize(IPacketHandler* handle)
{
	handle_ = handle;
}

bool RPCStub::packetProc(Session* session, Packet& packet, int type)
{
	switch (type) 
	{
	case 0: 
	{
		int id;
		char direction;
		short x;
		short y;
		char hp;

		packet >> id >> direction >> x >> y >> hp;

		return handle_->sc_create_my_character(session, id, direction, x, y, hp);
		break;
	}

	case 1: 
	{
		int id;
		char direction;
		short x;
		short y;
		char hp;

		packet >> id >> direction >> x >> y >> hp;

		return handle_->sc_create_other_character(session, id, direction, x, y, hp);
		break;
	}

	case 2: 
	{
		int id;

		packet >> id;

		return handle_->sc_character_delete(session, id);
		break;
	}

	case 10: 
	{
		char direction;
		short x;
		short y;

		packet >> direction >> x >> y;

		return handle_->cs_start_move(session, direction, x, y);
		break;
	}

	case 11: 
	{
		int id;
		char direction;
		short x;
		short y;

		packet >> id >> direction >> x >> y;

		return handle_->sc_start_move(session, id, direction, x, y);
		break;
	}

	case 12: 
	{
		char action;
		short x;
		short y;

		packet >> action >> x >> y;

		return handle_->cs_stop_move(session, action, x, y);
		break;
	}

	case 13: 
	{
		int id;
		char action;
		short x;
		short y;

		packet >> id >> action >> x >> y;

		return handle_->sc_stop_move(session, id, action, x, y);
		break;
	}

	case 20: 
	{
		char direction;
		short x;
		short y;

		packet >> direction >> x >> y;

		return handle_->cs_attack1(session, direction, x, y);
		break;
	}

	case 21: 
	{
		int id;
		char direction;
		short x;
		short y;

		packet >> id >> direction >> x >> y;

		return handle_->sc_attack1(session, id, direction, x, y);
		break;
	}

	case 22: 
	{
		char direction;
		short x;
		short y;

		packet >> direction >> x >> y;

		return handle_->cs_attack2(session, direction, x, y);
		break;
	}

	case 23: 
	{
		int id;
		char direction;
		short x;
		short y;

		packet >> id >> direction >> x >> y;

		return handle_->sc_attack2(session, id, direction, x, y);
		break;
	}

	case 24: 
	{
		char direction;
		short x;
		short y;

		packet >> direction >> x >> y;

		return handle_->cs_attack3(session, direction, x, y);
		break;
	}

	case 25: 
	{
		int id;
		char direction;
		short x;
		short y;

		packet >> id >> direction >> x >> y;

		return handle_->sc_attack3(session, id, direction, x, y);
		break;
	}

	case 30: 
	{
		int attackID;
		int damageID;
		char damageHP;

		packet >> attackID >> damageID >> damageHP;

		return handle_->sc_damage(session, attackID, damageID, damageHP);
		break;
	}

	default:
	{
		// 로그 추가

		return false;
	}
	}
}
