#include "UnityClient.h"
#include "../Utils/Packet.h"


UnityClient::UnityClient()
{
	rpc_.client_ = this;
}

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
	if (!PacketProc(packet))
	{
		//disconnect(sessionId);

		return;
	}

	delete packet;
}

void UnityClient::SendTest()
{
	///1
	//rpc_.LoginRequest(7777);

	///2
	//std::string s = "hello";

	//rpc_.ChatRequest(s);

	///3
	std::list<int> ls;
	ls.push_back(1);
	ls.push_back(2);
	ls.push_back(3);

	rpc_.ItemRequest(ls);
}


bool UnityClient::LoginRequest(int id)
{

	return true;
}

bool UnityClient::LoginResponse(int id)
{
	///echo
	std::cout << id << std::endl;

	return true;
}

bool UnityClient::ChatRequest(std::string& chat)
{

	return true;
}

bool UnityClient::ChatResponse(std::string& chat)
{
	std::cout << chat << std::endl;

	return true;
}

bool UnityClient::ItemRequest(std::list<int>& lst)
{

	return true;
}

bool UnityClient::ItemResponse(std::list<int>& lst)
{
	for (auto i : lst)
		std::cout << i << " ";

	std::cout << std::endl;

	return true;
}
