#include "MyServer.h"
#include "RpcModule.h"

bool MyServer::ReqUserRegisterDB(__int64 sessionId, std::string& loginId, std::string& password)
{
	RESPONSE_CODE code = userRepository_.CreateUser(loginId, password);

	dbProxy_.ResUserRegisterDB(sessionId, code);

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

	dbProxy_.ResUserLoginDB(sessionId, code, userId);

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

	dbProxy_.ResPlayerRegisterDB(sessionId, code);

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

	dbProxy_.ResPlayerProfileDB(sessionId, p);


	std::vector<Character> characters;

	inventoryRepository_.FindCharacterByPlayerId(p.playerId_, characters);

	dbProxy_.ResPlayerCharactersDB(sessionId, characters);


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
	newPlayer->Initialize(sessionId, player.playerId_, player.playerName_, player.level_, player.gold_, player.equippedInvenId_);

	if (!playerManager_.AddPlayer(sessionId, newPlayer))
	{
		delete newPlayer;

		// 클라이언트는 해당 패킷 수신시, 플레이어 등록 안되었음을 확인
		player.playerId_ = 0;

		g_RpcProxy.ResPlayerProfile(sessionId, player);

		return true;
	}

	g_RpcProxy.ResPlayerProfile(sessionId, player);


	// 플레이어에게 로비 플레이어들 정보 송신
	std::vector<PlayerInfo> infos;

	for (auto& p : playerManager_.GetPlayers())
	{
		PlayerInfo info(*(p.second));
		infos.push_back(info);
	}

	g_RpcProxy.ResLobbyPlayers(sessionId, infos);

	// 복사생성자 호출
	PlayerInfo info(player);

	// 다른 플레이어들에게도 입장 알림
	for (auto& p : playerManager_.GetPlayers())
	{
		g_RpcProxy.ResPlayerEnterLobby(p.second->sessionId_, info);
	}

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

bool MyServer::ReqBuyCharacterDB(__int64 sessionId, int playerId, int inventoryId, int characterId, int curGold)
{
	db_->BeginTransaction();

	playerRepository_.UpdateGold(playerId, curGold);
	inventoryRepository_.CreateCharacter(playerId, inventoryId, characterId);

	db_->Commit();

	return true;
}

bool MyServer::ReqChangeEquipmentDB(__int64 sessionId, int playerId, int inventoryId)
{
	playerRepository_.UpdateEquipment(playerId, inventoryId);

	return true;
}
