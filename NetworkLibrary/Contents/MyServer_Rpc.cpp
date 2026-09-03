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

	g_DbProxy.ReqUserRegisterDB(sessionId, loginId, password);

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

	g_DbProxy.ReqUserLoginDB(sessionId, loginId, password);

	return true;
}

bool MyServer::ReqPlayerRegister(__int64 sessionId, int userId, std::string& playerName)
{
	g_DbProxy.ReqPlayerRegisterDB(sessionId, userId, playerName);

	return true;
}

bool MyServer::ReqPlayerConnection(__int64 sessionId, int userId)
{
	g_DbProxy.ReqPlayerConnectionDB(sessionId, userId);

	return true;
}

bool MyServer::ReqEnterLobby(__int64 sessionId)
{
	Player* player = playerManager_.GetPlayerBySessionId(sessionId);

	if (player == nullptr)
		return true;

	player->state_ = PLAYER_STATE::LOBBY;


	// 플레이어에게 로비 플레이어들 정보 송신
	for (auto& p : playerManager_.GetPlayers())
	{
		// 복사 생성자
		PlayerInfo info(*(p.second));

		g_RpcProxy.ResEnterLobby(sessionId, info);
	}

	// TODO: 수정
	PlayerInfo info;
	info.playerId_ = player->playerId_;
	info.playerName_ = player->playerName_;
	info.level_ = player->level_;
	info.state_ = player->state_;


	// 다른 플레이어들에게도 입장 알림
	for (auto& p : playerManager_.GetPlayers())
	{
		if (p.second->playerId_ == player->playerId_)
			continue;

		g_RpcProxy.ResEnterLobby(p.second->sessionId_, info);
	}
}

bool MyServer::ReqChat(__int64 sessionId, std::string& message)
{
	Player* player = playerManager_.GetPlayerBySessionId(sessionId);

	if (player == nullptr)
		return true;

	unsigned int now = GetTickCount();

	// 1초 내에 다시 채팅 보냈을시
	if (now - player->lastChatTime_ < 1000)
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


	// 로비 내 사람들끼리 채팅
	if (player->state_ == PLAYER_STATE::LOBBY)
	{
		for (auto& i : playerManager_.GetPlayers())
		{
			if (i.second->state_ != PLAYER_STATE::LOBBY)
				continue;

			g_RpcProxy.ResChat(i.second->sessionId_, player->playerId_, message);
		}
	}
	// 게임룸 내 사람들끼리 채팅
	else if (player->state_ == PLAYER_STATE::GAMEROOM)
	{
		// TODO: 코드 개선 방안?...
		player->gameRoom_->BraodcastChatting(player, message);
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

	if (player->money_ < 1000)
	{
		Character ch{};
		ch.characterId_ = 0;

		g_RpcProxy.ResBuyCharacter(sessionId, ch, player->money_);

		return true;
	}

	player->money_ -= 1000;

	// TODO: 캐릭터 임의 생성 수정
	ch.characterId_ = 1;
	ch.inventoryId_ = inventoryRepository_.GenerateInventoryId();

	g_RpcProxy.ResBuyCharacter(sessionId, ch, player->money_);

	g_DbProxy.ReqBuyCharacterDB(sessionId, playerId, ch.inventoryId_, ch.characterId_, player->money_);

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

			g_DbProxy.ReqChangeEquipmentDB(sessionId, player->playerId_, inventoryId);

			break;
		}
	}

	return true;
}

bool MyServer::ReqStartMatch(__int64 sessionId)
{
	Player* player = playerManager_.GetPlayerBySessionId(sessionId);

	if (player == nullptr)
		return true;

	matchMaker_.Add(player);

	return true;
}

bool MyServer::ReqCancelMatch(__int64 sessionId)
{
	Player* player = playerManager_.GetPlayerBySessionId(sessionId);

	if (player == nullptr)
		return true;

	matchMaker_.Remove(player);

	return true;
}

bool MyServer::ReqPlaceStone(__int64 sessionId, short row, short col)
{
	Player* player = playerManager_.GetPlayerBySessionId(sessionId);

	if (player == nullptr)
		return true;

	player->gameRoom_->PlaceStone(player, row, col);

	return true;
}

bool MyServer::ReqLeaveRoom(__int64 sessionId)
{
	Player* player = playerManager_.GetPlayerBySessionId(sessionId);

	if (player == nullptr)
		return true;

	int roomId = player->gameRoom_->GetRoomId();

	if (player->gameRoom_->LeaveRoom(player))
		roomManager_.RemoveRoom(roomId);

	return true;
}
