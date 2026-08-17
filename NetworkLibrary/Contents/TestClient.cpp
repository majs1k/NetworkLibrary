#include "TestClient.h"
#include "../Utils/Packet.h"

void TestClient::OnConnect()
{

}

void TestClient::OnRelease()
{

}

void TestClient::OnRecv(Packet* packet)
{
	__int64 data;
	*packet >> data;

	delete packet;

	std::cout << data << std::endl;
}

void TestClient::SendTestNum()
{
	Packet* packet2 = new Packet();
	packet2->Initialize();

	*packet2 << seed_++;

	SendPacket(packet2);
}
