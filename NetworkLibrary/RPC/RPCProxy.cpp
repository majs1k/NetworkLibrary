#include "RPCProxy.h"
#include "../Network/LanServer.h"
#include "../Utils/Packet.h"

void RPCProxy::sc_create_my_character(__int64 sessionId, int id, char direction, short x, short y, char hp)
{
	Packet packet;

	packet << static_cast<unsigned char>(0);
	packet << id << direction << x << y << hp;

	server->sendPacket(sessionId, packet);
}

void RPCProxy::sc_create_other_character(__int64 sessionId, int id, char direction, short x, short y, char hp)
{
	Packet packet;

	packet << static_cast<unsigned char>(1);
	packet << id << direction << x << y << hp;

	server->sendPacket(sessionId, packet);
}

void RPCProxy::sc_character_delete(__int64 sessionId, int id)
{
	Packet packet;

	packet << static_cast<unsigned char>(2);
	packet << id;

	server->sendPacket(sessionId, packet);
}

void RPCProxy::cs_start_move(__int64 sessionId, char direction, short x, short y)
{
	Packet packet;

	packet << static_cast<unsigned char>(10);
	packet << direction << x << y;

	server->sendPacket(sessionId, packet);
}

void RPCProxy::sc_start_move(__int64 sessionId, int id, char direction, short x, short y)
{
	Packet packet;

	packet << static_cast<unsigned char>(11);
	packet << id << direction << x << y;

	server->sendPacket(sessionId, packet);
}

void RPCProxy::cs_stop_move(__int64 sessionId, char action, short x, short y)
{
	Packet packet;

	packet << static_cast<unsigned char>(12);
	packet << action << x << y;

	server->sendPacket(sessionId, packet);
}

void RPCProxy::sc_stop_move(__int64 sessionId, int id, char action, short x, short y)
{
	Packet packet;

	packet << static_cast<unsigned char>(13);
	packet << id << action << x << y;

	server->sendPacket(sessionId, packet);
}

void RPCProxy::cs_attack1(__int64 sessionId, char direction, short x, short y)
{
	Packet packet;

	packet << static_cast<unsigned char>(20);
	packet << direction << x << y;

	server->sendPacket(sessionId, packet);
}

void RPCProxy::sc_attack1(__int64 sessionId, int id, char direction, short x, short y)
{
	Packet packet;

	packet << static_cast<unsigned char>(21);
	packet << id << direction << x << y;

	server->sendPacket(sessionId, packet);
}

void RPCProxy::cs_attack2(__int64 sessionId, char direction, short x, short y)
{
	Packet packet;

	packet << static_cast<unsigned char>(22);
	packet << direction << x << y;

	server->sendPacket(sessionId, packet);
}

void RPCProxy::sc_attack2(__int64 sessionId, int id, char direction, short x, short y)
{
	Packet packet;

	packet << static_cast<unsigned char>(23);
	packet << id << direction << x << y;

	server->sendPacket(sessionId, packet);
}

void RPCProxy::cs_attack3(__int64 sessionId, char direction, short x, short y)
{
	Packet packet;

	packet << static_cast<unsigned char>(24);
	packet << direction << x << y;

	server->sendPacket(sessionId, packet);
}

void RPCProxy::sc_attack3(__int64 sessionId, int id, char direction, short x, short y)
{
	Packet packet;

	packet << static_cast<unsigned char>(25);
	packet << id << direction << x << y;

	server->sendPacket(sessionId, packet);
}

void RPCProxy::sc_damage(__int64 sessionId, int attackID, int damageID, char damageHP)
{
	Packet packet;

	packet << static_cast<unsigned char>(30);
	packet << attackID << damageID << damageHP;

	server->sendPacket(sessionId, packet);
}
