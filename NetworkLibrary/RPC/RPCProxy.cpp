#include "RPCProxy.h"
#include "../Network/Server.h"

void RPCProxy::sc_create_my_character(__int64 sessionId, int id, char direction, short x, short y, char hp)
{
	Packet packet;
	int size = 0 + sizeof(int) + sizeof(char) + sizeof(short) + sizeof(short) + sizeof(char);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)0;
	packet << id << direction << x << y << hp;

	Server::getInstance().sendPacket(sessionId, packet);
}

void RPCProxy::sc_create_other_character(__int64 sessionId, int id, char direction, short x, short y, char hp)
{
	Packet packet;
	int size = 0 + sizeof(int) + sizeof(char) + sizeof(short) + sizeof(short) + sizeof(char);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)1;
	packet << id << direction << x << y << hp;

	Server::getInstance().sendPacket(sessionId, packet);
}

void RPCProxy::sc_character_delete(__int64 sessionId, int id)
{
	Packet packet;
	int size = 0 + sizeof(int);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)2;
	packet << id;

	Server::getInstance().sendPacket(sessionId, packet);
}

void RPCProxy::cs_start_move(__int64 sessionId, char direction, short x, short y)
{
	Packet packet;
	int size = 0 + sizeof(char) + sizeof(short) + sizeof(short);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)10;
	packet << direction << x << y;

	Server::getInstance().sendPacket(sessionId, packet);
}

void RPCProxy::sc_start_move(__int64 sessionId, int id, char direction, short x, short y)
{
	Packet packet;
	int size = 0 + sizeof(int) + sizeof(char) + sizeof(short) + sizeof(short);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)11;
	packet << id << direction << x << y;

	Server::getInstance().sendPacket(sessionId, packet);
}

void RPCProxy::cs_stop_move(__int64 sessionId, char action, short x, short y)
{
	Packet packet;
	int size = 0 + sizeof(char) + sizeof(short) + sizeof(short);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)12;
	packet << action << x << y;

	Server::getInstance().sendPacket(sessionId, packet);
}

void RPCProxy::sc_stop_move(__int64 sessionId, int id, char action, short x, short y)
{
	Packet packet;
	int size = 0 + sizeof(int) + sizeof(char) + sizeof(short) + sizeof(short);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)13;
	packet << id << action << x << y;

	Server::getInstance().sendPacket(sessionId, packet);
}

void RPCProxy::cs_attack1(__int64 sessionId, char direction, short x, short y)
{
	Packet packet;
	int size = 0 + sizeof(char) + sizeof(short) + sizeof(short);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)20;
	packet << direction << x << y;

	Server::getInstance().sendPacket(sessionId, packet);
}

void RPCProxy::sc_attack1(__int64 sessionId, int id, char direction, short x, short y)
{
	Packet packet;
	int size = 0 + sizeof(int) + sizeof(char) + sizeof(short) + sizeof(short);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)21;
	packet << id << direction << x << y;

	Server::getInstance().sendPacket(sessionId, packet);
}

void RPCProxy::cs_attack2(__int64 sessionId, char direction, short x, short y)
{
	Packet packet;
	int size = 0 + sizeof(char) + sizeof(short) + sizeof(short);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)22;
	packet << direction << x << y;

	Server::getInstance().sendPacket(sessionId, packet);
}

void RPCProxy::sc_attack2(__int64 sessionId, int id, char direction, short x, short y)
{
	Packet packet;
	int size = 0 + sizeof(int) + sizeof(char) + sizeof(short) + sizeof(short);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)23;
	packet << id << direction << x << y;

	Server::getInstance().sendPacket(sessionId, packet);
}

void RPCProxy::cs_attack3(__int64 sessionId, char direction, short x, short y)
{
	Packet packet;
	int size = 0 + sizeof(char) + sizeof(short) + sizeof(short);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)24;
	packet << direction << x << y;

	Server::getInstance().sendPacket(sessionId, packet);
}

void RPCProxy::sc_attack3(__int64 sessionId, int id, char direction, short x, short y)
{
	Packet packet;
	int size = 0 + sizeof(int) + sizeof(char) + sizeof(short) + sizeof(short);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)25;
	packet << id << direction << x << y;

	Server::getInstance().sendPacket(sessionId, packet);
}

void RPCProxy::sc_damage(__int64 sessionId, int attackID, int damageID, char damageHP)
{
	Packet packet;
	int size = 0 + sizeof(int) + sizeof(int) + sizeof(char);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)30;
	packet << attackID << damageID << damageHP;

	Server::getInstance().sendPacket(sessionId, packet);
}
