#include "MyClient.h"
#include "../Utils/Packet.h"


MyClient::MyClient()
{
	rpc_.client_ = this;
	stub_.handler_ = this;
}

void MyClient::OnConnect()
{

}

void MyClient::OnRelease()
{

}

void MyClient::OnRecv(Packet* packet)
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


void MyClient::TestUserRegister()
{
	std::cout << "TestRegister" << std::endl;

	std::string loginId;
	std::string password;

	std::cout << "input id: ";
	std::cin >> loginId;
	std::cout << "input pw: ";
	std::cin >> password;


	rpc_.ReqUserRegister(loginId, password);
}

void MyClient::TestUserLogin()
{
	std::cout << "TestLogin" << std::endl;

	std::string loginId;
	std::string password;

	std::cout << "input loginId: ";
	std::cin >> loginId;
	std::cout << "input password: ";
	std::cin >> password;

	rpc_.ReqUserLogin(loginId, password);
}

void MyClient::TestCreatePlayer()
{
	std::cout << "TestCreatePlayer" << std::endl;

	int userId;
	std::string playerName;

	std::cout << "input userId: ";
	std::cin >> userId;
	std::cout << "input playerName: ";
	std::cin >> playerName;

	rpc_.ReqCreatePlayer(userId, playerName);
}

void MyClient::TestPlayerInfo()
{
	std::cout << "TestPlayerInfo" << std::endl;

	int userId;

	std::cout << "input userId: ";
	std::cin >> userId;

	rpc_.ReqPlayerProfile(userId);
}

void MyClient::TestPlayerList()
{
	std::cout << "TestPlayerList" << std::endl;

	rpc_.ReqPlayerList();
}

void MyClient::TestChat()
{
	std::cout << "TestChat" << std::endl;

	std::string message;

	std::cout << "input message: ";
	std::cin >> message;

	rpc_.ReqChat(message);
}

//------------------------------------------------------------------------------------------------------//

bool MyClient::ResUserRegister(RESPONSE_CODE code)
{
	std::cout << "ResRegister" << std::endl;

	std::cout << "code : " << (short)code << std::endl;

	std::cout << std::endl;

	return true;
}

bool MyClient::ResUserLogin(RESPONSE_CODE code, int userId)
{
	std::cout << "ResRegister" << std::endl;

	std::cout << "code : " << (short)code << std::endl;

	std::cout << "userId : " << (short)userId << std::endl;

	std::cout << std::endl;

	return true;
}


bool MyClient::ResCreatePlayer(RESPONSE_CODE code)
{
	std::cout << "ResCreatePlayer" << std::endl;

	std::cout << "code : " << (short)code << std::endl;

	std::cout << std::endl;

	return true;
}

bool MyClient::ResPlayerProfile(RESPONSE_CODE code, Player player)
{
	std::cout << "ResPlayerProfile" << std::endl;

	std::cout << "code : " << (short)code << std::endl;

	std::cout << "playerId : " << player.playerId_ << std::endl;

	std::cout << "playerName : " << player.playerName_ << std::endl;

	std::cout << "level : " << player.level_ << std::endl;

	std::cout << "gold : " << player.gold_ << std::endl;

	std::cout << std::endl;

	return true;
}

bool MyClient::ResPlayerList(std::list<Player> playerList)
{
	std::cout << "ResPlayerList" << std::endl;

	for (auto& p : playerList)
	{
		std::cout << "playerId : " << p.playerId_ << std::endl;

		std::cout << "playerName : " << p.playerName_ << std::endl;

		std::cout << "level : " << p.level_ << std::endl;

		std::cout << "gold : " << p.gold_ << std::endl;

		std::cout << std::endl;
	}

	std::cout << std::endl;

	return true;
}

bool MyClient::ResChat(int playerId, std::string& message)
{
	std::cout << "ResChat" << std::endl;

	std::cout << "playerId : " << playerId << std::endl;

	std::cout << "message : " << message << std::endl;

	std::cout << std::endl;

	return true;
}
