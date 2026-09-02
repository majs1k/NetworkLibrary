#include "MyServer.h"
#include "RpcModule.h"

bool MyServer::ReqUserRegister(__int64 sessionId, std::string& loginId, std::string& password)
{
	// 길이 제한
	if (loginId.length() > 20 || password.length() > 20)
	{
		RESPONSE_CODE c = RESPONSE_CODE::INPUT_LENGTH_OVER;
		g_RpcProxy.ResUserRegister(sessionId, c);

		return true;
	}

	dbProxy_.ReqUserRegisterDB(sessionId, loginId, password);

	return true;
}

bool MyServer::ReqUserLogin(__int64 sessionId, std::string& loginId, std::string& password)
{
	if (loginId.length() > 20 || password.length() > 20)
	{
		RESPONSE_CODE c = RESPONSE_CODE::INPUT_LENGTH_OVER;
		g_RpcProxy.ResUserRegister(sessionId, c);

		return true;
	}

	dbProxy_.ReqUserLoginDB(sessionId, loginId, password);

	return true;
}

bool MyServer::ReqPlayerRegister(__int64 sessionId, int userId, std::string& playerName)
{
	dbProxy_.ReqPlayerRegisterDB(sessionId, userId, playerName);

	return true;
}

bool MyServer::ReqPlayerConnection(__int64 sessionId, int userId)
{
	dbProxy_.ReqPlayerConnectionDB(sessionId, userId);

	return true;
}

bool MyServer::ReqChat(__int64 sessionId, std::string& message)
{
	Player* player = playerManager_.GetPlayerBySessionId(sessionId);

	if (player == nullptr)
		return true;

	if (player->state_ != PLAYER_STATE::LOBBY)
		return true;

	unsigned int now = GetTickCount();

	// 1초 내에 다시 채팅 보냈을시
	if(now - player->lastChatTime_ < 1000)
	{
		player->curChatCount_++;
	}

	// 채팅 도배 시 확인시점으로부터 5초간 금지
	if (now - player->lastChatTime_ >= 5000)
	{
		player->curChatCount_ = 0;
	}

	// 채팅 도배 금지
	if (player->curChatCount_ > 5)
	{
		std::string m = u8"잠시 후 다시 채팅해주세요.";
		g_RpcProxy.ResChat(sessionId, player->playerId_, m);

		return true;
	}

	player->lastChatTime_ = now;

	for (auto& i : playerManager_.GetPlayers())
	{
		g_RpcProxy.ResChat(i.second->sessionId_, player->playerId_, message);
	}

	return true;
}

bool MyServer::ReqBuyCharacter(__int64 sessionId)
{
	Player* player = playerManager_.GetPlayerBySessionId(sessionId);

	if (player == nullptr)
		return true;

	int playerId = player->playerId_;

	if (player->state_ != PLAYER_STATE::LOBBY)
		return true;

	Character ch{};

	if (player->gold_ < 1000)
	{
		Character ch{};
		ch.characterId_ = 0;

		g_RpcProxy.ResBuyCharacter(sessionId, ch, player->gold_);

		return true;
	}

	player->gold_ -= 1000;

	// TODO: 캐릭터 임의 생성 수정
	ch.characterId_ = 1;
	ch.inventoryId_ = inventoryRepository_.GenerateInventoryId();

	g_RpcProxy.ResBuyCharacter(sessionId, ch, player->gold_);

	dbProxy_.ReqBuyCharacterDB(sessionId, playerId, ch.inventoryId_, ch.characterId_, player->gold_);

	return true;
}

bool MyServer::ReqChangeEquipment(__int64 sessionId, int inventoryId)
{
	Player* player = playerManager_.GetPlayerBySessionId(sessionId);

	if (player == nullptr)
		return true;

	// 플레이어가 로비가 아니라면 리턴
	if (player->state_ != PLAYER_STATE::LOBBY)
		return true;

	// 해당 캐릭터 id를 보유하고 있을시, db 저장
	for (auto& c : player->characters_)
	{
		if (c.inventoryId_ == inventoryId)
		{
			g_RpcProxy.ResChangeEquipment(sessionId, inventoryId);

			dbProxy_.ReqChangeEquipmentDB(sessionId, player->playerId_, inventoryId);

			break;
		}
	}

	return true;
}

bool MyServer::ReqStartGame(__int64 sessionId)
{
	Player* player = playerManager_.GetPlayerBySessionId(sessionId);

	if (player == nullptr)
		return true;

	matchMaker_.Add(player);

	return true;
}

bool MyServer::ReqCancelGame(__int64 sessionId)
{
	Player* player = playerManager_.GetPlayerBySessionId(sessionId);

	if (player == nullptr)
		return true;

	matchMaker_.Remove(player);

	return true;
}
