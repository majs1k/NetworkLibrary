#include "MyServer.h"
#include "RpcModule.h"

bool MyServer::ReqUserRegisterDB(__int64 sessionId, std::string& loginId, std::string& password)
{
	RESPONSE_CODE code = userRepository_.CreateUser(loginId, password);

	g_DbProxy.ResUserRegisterDB(sessionId, code);

	return true;
}

bool MyServer::ResUserRegisterDB(__int64 sessionId, RESPONSE_CODE code)
{
	g_RpcProxy.ResUserRegister(sessionId, code);

	return true;
}

bool MyServer::ReqUserLoginDB(__int64 sessionId, std::string& loginId, std::string& password)
{
	int userId = 0;

	RESPONSE_CODE code = userRepository_.FindByLoginId(loginId, password, userId);

	g_DbProxy.ResUserLoginDB(sessionId, code, userId);

	return true;
}

bool MyServer::ResUserLoginDB(__int64 sessionId, RESPONSE_CODE code, int userId)
{
	g_RpcProxy.ResUserLogin(sessionId, code, userId);

	return true;
}


bool MyServer::ReqPlayerRegisterDB(__int64 sessionId, int userId, std::string& playerName)
{
	RESPONSE_CODE code = playerRepository_.CreatePlayer(userId, playerName);

	g_DbProxy.ResPlayerRegisterDB(sessionId, code);

	return true;
}

bool MyServer::ResPlayerRegisterDB(__int64 sessionId, RESPONSE_CODE code)
{
	g_RpcProxy.ResPlayerRegister(sessionId, code);

	return true;
}

bool MyServer::ReqPlayerConnectionDB(__int64 sessionId, int userId)
{
	Player p;
	//TODO: 클라이언트 측에선 playerId 0으로 수신시 오류메세지 띄우기
	p.playerId_ = 0;

	playerRepository_.FindPlayerByUserId(userId, p);

	// 잘못된 요청 (해당 유저아이디의 플레이어 없음)
	if (p.playerId_ == 0)
	{
		g_RpcProxy.ResPlayerProfile(sessionId, p);

		return true;
	}

	g_DbProxy.ResPlayerProfileDB(sessionId, p);


	std::vector<Character> characters;

	inventoryRepository_.FindCharacterByPlayerId(p.playerId_, characters);

	g_DbProxy.ResPlayerCharactersDB(sessionId, characters);


	return true;
}

bool MyServer::ResPlayerProfileDB(__int64 sessionId, Player& player)
{
	// 잘못된 요청 (플레이어 이미 서버 접속)
	if (playerManager_.ContainsPlayer(player.playerId_))
	{
		player.playerId_ = -1;

		g_RpcProxy.ResPlayerProfile(sessionId, player);

		return true;
	}

	Player* newPlayer = new Player();
	newPlayer->Initialize(sessionId, player.playerId_, player.playerName_, player.level_, player.money_, player.equippedInvenId_);

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

bool MyServer::ResPlayerCharactersDB(__int64 sessionId, std::vector<Character>& characters)
{
	Player* p = playerManager_.GetPlayerBySessionId(sessionId);

	if (p == nullptr)
		return true;

	// 플레이어의 캐릭터 등록
	for (auto& c : characters)
	{
		// TODO: emplace_back???
		p->characters_.push_back(c);
	}

	g_RpcProxy.ResPlayerCharacters(sessionId, characters);

	return true;
}

bool MyServer::ReqBuyCharacterDB(__int64 sessionId, int playerId, int inventoryId, int characterId, int curMoney)
{
	db_->BeginTransaction();

	playerRepository_.UpdateMoney(playerId, curMoney);
	inventoryRepository_.CreateCharacter(playerId, inventoryId, characterId);

	db_->Commit();

	return true;
}

bool MyServer::ReqChangeEquipmentDB(__int64 sessionId, int playerId, int inventoryId)
{
	playerRepository_.UpdateEquipment(playerId, inventoryId);

	return true;
}

bool MyServer::ReqGameResultDB(__int64 sessionId, int winnerId, int loserId)
{
	Player* winner = playerManager_.GetPlayerBySessionId(winnerId);

	if (winner == nullptr)
		return true;

	Player* loser = playerManager_.GetPlayerBySessionId(loserId);

	if (loser == nullptr)
		return true;

	db_->BeginTransaction();

	playerRepository_.UpdateLevel(winnerId, winner->level_);
	playerRepository_.UpdateMoney(winnerId, winner->money_);

	db_->Commit();

	return true;
}
