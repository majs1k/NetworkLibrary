#include "RPCStub.h"
#include "../Utils/Packet.h"

IPacketHandler::~IPacketHandler()
{

}

bool IPacketHandler::sc_create_my_character(__int64 sessionId, int id, char direction, short x, short y, char hp)
{
	return true;
}

bool IPacketHandler::sc_create_other_character(__int64 sessionId, int id, char direction, short x, short y, char hp)
{
	return true;
}

bool IPacketHandler::sc_character_delete(__int64 sessionId, int id)
{
	return true;
}

bool IPacketHandler::cs_start_move(__int64 sessionId, char direction, short x, short y)
{
	return true;
}

bool IPacketHandler::sc_start_move(__int64 sessionId, int id, char direction, short x, short y)
{
	return true;
}

bool IPacketHandler::cs_stop_move(__int64 sessionId, char action, short x, short y)
{
	return true;
}

bool IPacketHandler::sc_stop_move(__int64 sessionId, int id, char action, short x, short y)
{
	return true;
}

bool IPacketHandler::cs_attack1(__int64 sessionId, char direction, short x, short y)
{
	return true;
}

bool IPacketHandler::sc_attack1(__int64 sessionId, int id, char direction, short x, short y)
{
	return true;
}

bool IPacketHandler::cs_attack2(__int64 sessionId, char direction, short x, short y)
{
	return true;
}

bool IPacketHandler::sc_attack2(__int64 sessionId, int id, char direction, short x, short y)
{
	return true;
}

bool IPacketHandler::cs_attack3(__int64 sessionId, char direction, short x, short y)
{
	return true;
}

bool IPacketHandler::sc_attack3(__int64 sessionId, int id, char direction, short x, short y)
{
	return true;
}

bool IPacketHandler::sc_damage(__int64 sessionId, int attackID, int damageID, char damageHP)
{
	return true;
}

void RPCStub::initialize(IPacketHandler* handle)
{
	handle_ = handle;
}

bool RPCStub::packetProc(__int64 sessionId, Packet& packet, unsigned char type)
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

		return handle_->sc_create_my_character(sessionId, id, direction, x, y, hp);
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

		return handle_->sc_create_other_character(sessionId, id, direction, x, y, hp);
		break;
	}

	case 2: 
	{
		int id;

		packet >> id;

		return handle_->sc_character_delete(sessionId, id);
		break;
	}

	case 10: 
	{
		char direction;
		short x;
		short y;

		packet >> direction >> x >> y;

		return handle_->cs_start_move(sessionId, direction, x, y);
		break;
	}

	case 11: 
	{
		int id;
		char direction;
		short x;
		short y;

		packet >> id >> direction >> x >> y;

		return handle_->sc_start_move(sessionId, id, direction, x, y);
		break;
	}

	case 12: 
	{
		char action;
		short x;
		short y;

		packet >> action >> x >> y;

		return handle_->cs_stop_move(sessionId, action, x, y);
		break;
	}

	case 13: 
	{
		int id;
		char action;
		short x;
		short y;

		packet >> id >> action >> x >> y;

		return handle_->sc_stop_move(sessionId, id, action, x, y);
		break;
	}

	case 20: 
	{
		char direction;
		short x;
		short y;

		packet >> direction >> x >> y;

		return handle_->cs_attack1(sessionId, direction, x, y);
		break;
	}

	case 21: 
	{
		int id;
		char direction;
		short x;
		short y;

		packet >> id >> direction >> x >> y;

		return handle_->sc_attack1(sessionId, id, direction, x, y);
		break;
	}

	case 22: 
	{
		char direction;
		short x;
		short y;

		packet >> direction >> x >> y;

		return handle_->cs_attack2(sessionId, direction, x, y);
		break;
	}

	case 23: 
	{
		int id;
		char direction;
		short x;
		short y;

		packet >> id >> direction >> x >> y;

		return handle_->sc_attack2(sessionId, id, direction, x, y);
		break;
	}

	case 24: 
	{
		char direction;
		short x;
		short y;

		packet >> direction >> x >> y;

		return handle_->cs_attack3(sessionId, direction, x, y);
		break;
	}

	case 25: 
	{
		int id;
		char direction;
		short x;
		short y;

		packet >> id >> direction >> x >> y;

		return handle_->sc_attack3(sessionId, id, direction, x, y);
		break;
	}

	case 30: 
	{
		int attackID;
		int damageID;
		char damageHP;

		packet >> attackID >> damageID >> damageHP;

		return handle_->sc_damage(sessionId, attackID, damageID, damageHP);
		break;
	}

	default:
	{
		// 로그 추가

		return false;
	}
	}
}
