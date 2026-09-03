#pragma once
#include <vector>
#include "../../Network/LanClient.h"

#include "../ResponseCode.h"
#include "../Player.h"
#include "../Icon.h"

//#include "../RPC/RpcClientProxy.h"
#include "../../RPC/RpcClientStub.h"

#include "../OmokGame.h"

enum class SCENE
{
	LOGIN,
	PLAYER_REGISTER,
	MAIN,
	GAME
};

class RpcClientProxy;


class GameClient : public LanClient, public RpcClientStub
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

	bool ResUserRegister(RESPONSE_CODE code) override;
	bool ResUserLogin(RESPONSE_CODE code, int userId) override;

	bool ResPlayerRegister(RESPONSE_CODE code) override;


	bool ResPlayerProfile(Player& player) override;

	bool ResPlayerIcons(std::vector<Icon>& icons) override;


	bool ResChat(int playerId, std::string& message) override;


	bool ResEnterLobby(PlayerInfo& player) override;
	bool ResLeaveLobby(int playerId) override;


	bool ResBuyIcon(Icon& icon, int curMoney) override;

	bool ResChangeEquipment(int inventoryId) override;


	bool ResStartMatch(STONE stone, PlayerInfo& opponent) override;


	bool ResPlaceStone(short row, short col) override;

	bool ResGameResult(STONE winner, int myRating, int opponentRating, int myMoney) override;


	bool ResChangePlayerRating(int playerId, int rating) override;

	bool ResChangePlayerState(int playerId, PLAYER_STATE state) override;


	bool ResLeaveRoom(int playerId) override;
};
