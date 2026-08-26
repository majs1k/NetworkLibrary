#pragma once
#include <list>
#include "../Network/LanClient.h"

#include "ResponseCode.h"

#include "Player.h"
#include "Character.h"

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

	void TestPlayerRegister();

	void TestPlayerEnterGame();

	void TestChat();

	void TestLobbyPlayers();



	// ----------------------------------------------------- //

	bool ResUserRegister(RESPONSE_CODE code);
	bool ResUserLogin(RESPONSE_CODE code, int userId);

	bool ResPlayerRegister(RESPONSE_CODE code);


	bool ResPlayerProfile(Player player);

	bool ResPlayerCharacters(std::list<Character> characterList);


	bool ResChat(int playerId, std::string& message);

	bool ResLobbyPlayers(std::list<Player> playerList);
};
