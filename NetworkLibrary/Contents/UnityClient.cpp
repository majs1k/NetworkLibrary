#include "UnityClient.h"
#include "../Utils/Packet.h"


UnityClient::UnityClient()
{
	rpc_.client_ = this;
	stub_.handler_ = this;
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
	if (!stub_.PacketProc(packet))
	{
		//disconnect(sessionId);

		return;
	}

	delete packet;
}


void UnityClient::TestRegister(std::string& loginId, std::string& password)
{
	rpc_.ReqRegister(loginId, password);

	///2
	//std::string s = "hello";

	//rpc_.ReqChat(s);
}

void UnityClient::TestLogin(std::string& loginId, std::string& password)
{
	rpc_.ReqLogin(loginId, password);

}

bool UnityClient::ReqRegister(std::string& loginId, std::string& password)
{
	return true;
}

bool UnityClient::ResRegister(RESPONSE_CODE code)
{
	std::cout << (short)code << std::endl;

	return true;
}

bool UnityClient::ReqLogin(std::string& loginId, std::string& password)
{
	return true;
}

bool UnityClient::ResLogin(RESPONSE_CODE code)
{
	std::cout << (short)code << std::endl;

	return true;
}
