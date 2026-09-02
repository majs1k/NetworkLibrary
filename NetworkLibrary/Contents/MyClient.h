#pragma once
#include <vector>
#include "../Network/LanClient.h"

#include "ResponseCode.h"
#include "Player.h"
#include "Character.h"

//#include "../RPC/RpcClientProxy.h"
#include "../RPC/RpcClientStub.h"


// TODO: 클래스 분리
enum class SCENE
{
	LOGIN,
	PLAYER_REGISTER,
	MAIN,
	GAME
};

class RpcClientProxy;


class MyClient : public LanClient, public RpcClientStub
{
	friend class GameUI;

public:

	void AttachProxy(RpcClientProxy* rpcProxy);

	void OnConnect() override;
	void OnRelease() override;
	void OnRecv(Packet* packet) override;

	// ===================================================================================== //
	// Contents
	// ===================================================================================== //

private:

	SCENE scene_ = SCENE::LOGIN;


	char loginSceneStatus_[64];
	int userId_ = 0;

	char playerSceneStatus_[64];


	Player myPlayer_;
	std::unordered_map<int, PlayerInfo> playerMap_{};

	std::vector<std::string> chatMessages_;


	PlayerInfo enemyPlayer_;
	Character myCharacter_;
	Character enemyCharacter_;

	// ===================================================================================== //
	// RPC
	// ===================================================================================== //

private:

	bool ResUserRegister(RESPONSE_CODE code);
	bool ResUserLogin(RESPONSE_CODE code, int userId);

	bool ResPlayerRegister(RESPONSE_CODE code);


	bool ResPlayerProfile(Player& player);

	bool ResPlayerCharacters(std::vector<Character>& characters);


	bool ResChat(int playerId, std::string& message);

	bool ResLobbyPlayers(std::vector<PlayerInfo>& players);


	bool ResPlayerEnterLobby(PlayerInfo& player);
	bool ResPlayerLeaveLobby(int playerId);


	bool ResBuyCharacter(Character& character, int curGold);

	bool ResChangeEquipment(int inventoryId);


	bool ResStartGame(PlayerInfo& otherPlayer);
};
