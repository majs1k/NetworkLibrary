#include "RPCProxy.h"
#include "../Network/Session.h"

void RPCProxy::sc_create_my_character(Session* session, int id, char direction, short x, short y, char hp)
{
	Packet packet;
	int size = 0 + sizeof(int) + sizeof(char) + sizeof(short) + sizeof(short) + sizeof(char);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)0;
	packet << id << direction << x << y << hp;

	session->sendPacket(packet);
}

void RPCProxy::sc_create_other_character(Session* session, int id, char direction, short x, short y, char hp)
{
	Packet packet;
	int size = 0 + sizeof(int) + sizeof(char) + sizeof(short) + sizeof(short) + sizeof(char);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)1;
	packet << id << direction << x << y << hp;

	session->sendPacket(packet);
}

void RPCProxy::sc_character_delete(Session* session, int id)
{
	Packet packet;
	int size = 0 + sizeof(int);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)2;
	packet << id;

	session->sendPacket(packet);
}

void RPCProxy::cs_start_move(Session* session, char direction, short x, short y)
{
	Packet packet;
	int size = 0 + sizeof(char) + sizeof(short) + sizeof(short);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)10;
	packet << direction << x << y;

	session->sendPacket(packet);
}

void RPCProxy::sc_start_move(Session* session, int id, char direction, short x, short y)
{
	Packet packet;
	int size = 0 + sizeof(int) + sizeof(char) + sizeof(short) + sizeof(short);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)11;
	packet << id << direction << x << y;

	session->sendPacket(packet);
}

void RPCProxy::cs_stop_move(Session* session, char action, short x, short y)
{
	Packet packet;
	int size = 0 + sizeof(char) + sizeof(short) + sizeof(short);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)12;
	packet << action << x << y;

	session->sendPacket(packet);
}

void RPCProxy::sc_stop_move(Session* session, int id, char action, short x, short y)
{
	Packet packet;
	int size = 0 + sizeof(int) + sizeof(char) + sizeof(short) + sizeof(short);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)13;
	packet << id << action << x << y;

	session->sendPacket(packet);
}

void RPCProxy::cs_attack1(Session* session, char direction, short x, short y)
{
	Packet packet;
	int size = 0 + sizeof(char) + sizeof(short) + sizeof(short);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)20;
	packet << direction << x << y;

	session->sendPacket(packet);
}

void RPCProxy::sc_attack1(Session* session, int id, char direction, short x, short y)
{
	Packet packet;
	int size = 0 + sizeof(int) + sizeof(char) + sizeof(short) + sizeof(short);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)21;
	packet << id << direction << x << y;

	session->sendPacket(packet);
}

void RPCProxy::cs_attack2(Session* session, char direction, short x, short y)
{
	Packet packet;
	int size = 0 + sizeof(char) + sizeof(short) + sizeof(short);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)22;
	packet << direction << x << y;

	session->sendPacket(packet);
}

void RPCProxy::sc_attack2(Session* session, int id, char direction, short x, short y)
{
	Packet packet;
	int size = 0 + sizeof(int) + sizeof(char) + sizeof(short) + sizeof(short);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)23;
	packet << id << direction << x << y;

	session->sendPacket(packet);
}

void RPCProxy::cs_attack3(Session* session, char direction, short x, short y)
{
	Packet packet;
	int size = 0 + sizeof(char) + sizeof(short) + sizeof(short);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)24;
	packet << direction << x << y;

	session->sendPacket(packet);
}

void RPCProxy::sc_attack3(Session* session, int id, char direction, short x, short y)
{
	Packet packet;
	int size = 0 + sizeof(int) + sizeof(char) + sizeof(short) + sizeof(short);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)25;
	packet << id << direction << x << y;

	session->sendPacket(packet);
}

void RPCProxy::sc_damage(Session* session, int attackID, int damageID, char damageHP)
{
	Packet packet;
	int size = 0 + sizeof(int) + sizeof(int) + sizeof(char);

	packet << (unsigned char)0x89 << (unsigned char)size << (unsigned char)30;
	packet << attackID << damageID << damageHP;

	session->sendPacket(packet);
}
