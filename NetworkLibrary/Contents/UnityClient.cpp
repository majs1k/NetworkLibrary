#include "UnityClient.h"
#include "../Utils/Packet.h"

void UnityClient::OnConnect()
{

}

void UnityClient::OnRelease()
{

}

void UnityClient::OnRecv(Packet* packet)
{
	__int64 sessionId = 0;

	// 함수의 인자 자료형 주의
	if (!PacketProc(sessionId, packet))
	{
		//disconnect(sessionId);

		return;
	}

	delete packet;
}

void UnityClient::SendTest()
{
	/// 프록시 코드 대체 
	Packet* packet = new Packet();
	packet->Initialize();

	int num = 7777;

	packet->GetHeaderPtr()->type_ = 0;
	*packet << num;

	//server_->SendPacket(sessionId, packet);
	SendPacket(packet);
}


bool UnityClient::LoginRequest(__int64 sessionId, int id)
{
	//proxy_.sc_create_login(sessionId, num);


	return true;
}

bool UnityClient::LoginResponse(__int64 sessionId, int id)
{
	///echo
	std::cout << id << std::endl;

	return true;
}
