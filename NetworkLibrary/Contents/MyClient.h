#pragma once
#include <list>
#include "../Network/LanClient.h"

#include "ResponseCode.h"

#include "Player.h"

#include "../RPC/RpcClientProxy.h"
#include "../RPC/RpcClientStub.h"



class MyClient : public LanClient, public RpcClientHandler
{
private:
	RpcClientProxy rpc_;
	RpcClientStub stub_;

public:
	MyClient();

	void OnConnect() override;
	void OnRelease() override;
	void OnRecv(Packet* packet) override;

	// ----------------------------------------------------- //

	void TestUserRegister();
	void TestUserLogin();

	void TestCreatePlayer();
	void TestPlayerInfo();

	void TestPlayerList();

	void TestChat();


	// ----------------------------------------------------- //

	bool ResUserRegister(RESPONSE_CODE code);
	bool ResUserLogin(RESPONSE_CODE code, int userId);

	bool ResCreatePlayer(RESPONSE_CODE code);
	bool ResPlayerProfile(RESPONSE_CODE code, Player player);

	bool ResPlayerList(std::list<Player> playerList);

	bool ResChat(int playerId, std::string& message);
};
