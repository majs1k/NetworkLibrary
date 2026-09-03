#include "GameServer.h"
#include "../RpcModule.h"

bool GameServer::ReqUserRegisterDB(__int64 sessionId, std::string& loginId, std::string& password)
{
	RESPONSE_CODE code = userRepository_.CreateUser(loginId, password);

	g_DbProxy.ResUserRegisterDB(sessionId, code);

	return true;
}

bool GameServer::ResUserRegisterDB(__int64 sessionId, RESPONSE_CODE code)
{
	g_RpcProxy.ResUserRegister(sessionId, code);

	return true;
}

bool GameServer::ReqUserLoginDB(__int64 sessionId, std::string& loginId, std::string& password)
{
	int userId = 0;

	RESPONSE_CODE code = userRepository_.FindByLoginId(loginId, password, userId);

	g_DbProxy.ResUserLoginDB(sessionId, code, userId);

	return true;
}

bool GameServer::ResUserLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId)
{
	g_RpcProxy.ResUserLogin(sessionId, code, userId);

	return true;
}


bool GameServer::ReqPlayerRegisterDB(__int64 sessionId, int userId, std::string& playerName)
{
	RESPONSE_CODE code = playerRepository_.CreatePlayer(userId, playerName);

	g_DbProxy.ResPlayerRegisterDB(sessionId, code);

	return true;
}

bool GameServer::ResPlayerRegisterDB(__int64 sessionId, RESPONSE_CODE code)
{
	g_RpcProxy.ResPlayerRegister(sessionId, code);

	return true;
}

bool GameServer::ReqPlayerConnectionDB(__int64 sessionId, int userId)
{
	Player p;
	// 클라이언트 측에선 playerId 0일시 예외처리
	p.playerId_ = 0;

	playerRepository_.FindPlayerByUserId(userId, p);

	// 잘못된 요청 (해당 유저아이디의 플레이어 없음)
	if (p.playerId_ == 0)
	{
		g_RpcProxy.ResPlayerProfile(sessionId, p);

		return true;
	}

	g_DbProxy.ResPlayerProfileDB(sessionId, p);


	std::vector<Icon> icons;

	inventoryRepository_.FindIconsByPlayerId(p.playerId_, icons);

	g_DbProxy.ResPlayerIconsDB(sessionId, icons);


	return true;
}

bool GameServer::ResPlayerProfileDB(__int64 sessionId, Player& player)
{
	// 잘못된 요청 (플레이어 이미 서버 접속)
	if (playerManager_.ContainsPlayer(player.playerId_))
	{
		player.playerId_ = -1;

		g_RpcProxy.ResPlayerProfile(sessionId, player);

		return true;
	}

	Player* newPlayer = new Player();
	newPlayer->Initialize(sessionId, player.playerId_, player.playerName_, player.win_, player.lose_, player.rating_, player.money_, player.equippedInvenId_);

	if (!playerManager_.AddPlayer(sessionId, newPlayer))
	{
		delete newPlayer;

		// 잘못된 요청 (플레이어 db에 등록 x)
		player.playerId_ = 0;

		g_RpcProxy.ResPlayerProfile(sessionId, player);

		return true;
	}

	g_RpcProxy.ResPlayerProfile(sessionId, player);

	return true;
}

bool GameServer::ResPlayerIconsDB(__int64 sessionId, std::vector<Icon>& icons)
{
	Player* p = playerManager_.GetPlayerBySessionId(sessionId);

	if (p == nullptr)
		return true;

	// 플레이어의 캐릭터 등록
	for (auto& i : icons)
	{
		// TODO: emplace_back???
		p->icons_.push_back(i);
	}

	g_RpcProxy.ResPlayerIcons(sessionId, icons);

	return true;
}

bool GameServer::ReqBuyIconDB(__int64 sessionId, int playerId, int inventoryId, int iconId, int curMoney)
{
	db_->BeginTransaction();

	playerRepository_.UpdateMoney(playerId, curMoney);
	inventoryRepository_.CreateIcon(playerId, inventoryId, iconId);

	db_->Commit();

	return true;
}

bool GameServer::ReqChangeEquipmentDB(__int64 sessionId, int playerId, int inventoryId)
{
	playerRepository_.UpdateProfileIcon(playerId, inventoryId);

	return true;
}

bool GameServer::ReqGameResultDB(__int64 sessionId, int winnerId, int loserId)
{
	Player* winner = playerManager_.GetPlayerBySessionId(winnerId);

	if (winner == nullptr)
		return true;

	Player* loser = playerManager_.GetPlayerBySessionId(loserId);

	if (loser == nullptr)
		return true;

	db_->BeginTransaction();

	// TODO: 개선점?
	playerRepository_.UpdateWin(winnerId, winner->win_);
	playerRepository_.UpdateMoney(winnerId, winner->money_);
	playerRepository_.UpdateRating(winnerId, winner->rating_);

	playerRepository_.UpdateLose(loserId, loser->lose_);
	playerRepository_.UpdateRating(loserId, loser->rating_);

	db_->Commit();

	return true;
}
