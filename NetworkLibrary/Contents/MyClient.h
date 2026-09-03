#pragma once
#include <vector>
#include "../Network/LanClient.h"

#include "ResponseCode.h"
#include "Player.h"
#include "Character.h"

//#include "../RPC/RpcClientProxy.h"
#include "../RPC/RpcClientStub.h"

#include "OmokGame.h"

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

	PlayerInfo opponentPlayer_;

	OmokGame omokGame_;


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


	bool ResEnterLobby(PlayerInfo& player);
	bool ResLeaveLobby(int playerId);


	bool ResBuyCharacter(Character& character, int curMoney);

	bool ResChangeEquipment(int inventoryId);


	virtual bool ResStartMatch(STONE stone, PlayerInfo& opponent);


	virtual bool ResPlaceStone(short row, short col);

	virtual bool ResGameResult(STONE stone, int level, int myMoney);



	virtual bool ResChangePlayerLevel(int playerId, int level);
	virtual bool ResChangePlayerState(int playerId, PLAYER_STATE state);


	virtual bool ResLeaveRoom(int playerId);
};
