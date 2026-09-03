#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Character.h"
#include "OmokGame.h"

enum class PLAYER_STATE : short
{
	NONE,
	LOBBY,
	GAMEROOM,
};

class RpcServerProxy;
class GameRoom;

struct Player
{
	__int64 sessionId_;

	int playerId_;
	std::string playerName_;
	int level_;
	int money_;

	PLAYER_STATE state_;

	std::vector<Character> characters_;
	int equippedInvenId_;

	GameRoom* gameRoom_;

	unsigned int lastChatTime_;
	int curChatCount_;

	STONE stone_;


	void Initialize(__int64 sessionId, int playerId, std::string& playerName, int level, int money, int equippedId)
	{
		sessionId_ = sessionId;

		playerId_ = playerId;
		playerName_ = playerName;
		level_ = level;
		money_ = money;

		state_ = PLAYER_STATE::NONE;

		characters_.clear();
		equippedInvenId_ = equippedId;

		gameRoom_ = nullptr;

		lastChatTime_ = 0;
		curChatCount_ = 0;
	}
};


class PlayerManager
{

private:

	// map 사용시, if(map[x] == ...) 이런식으로 확인 금지 (새 원소 만듦)
	// TODO: unique_ptr 적용
	std::unordered_map<__int64, int> sessionToPlayer_;
	std::unordered_map<int, Player*> playerMap_;

public:

	bool AddPlayer(__int64 sessionId, Player* player)
	{
		if (player == nullptr)
			return false;

		int playerId = player->playerId_;

		// 이미 존재하는 플레이어
		if (sessionToPlayer_.count(sessionId) != 0)
			return false;

		if (playerMap_.count(playerId) != 0)
			return false;

		sessionToPlayer_.insert({ sessionId, playerId });
		playerMap_.insert({ playerId, player });

		return true;
	}

	bool RemovePlayer(Player* player)
	{
		auto it = playerMap_.find(player->playerId_);

		if (it == playerMap_.end())
			return false;

		playerMap_.erase(player->playerId_);
		sessionToPlayer_.erase(player->sessionId_);

		delete player;

		return true;
	}

	// sessionId -> Player
	Player* GetPlayerBySessionId(__int64 sessionId)
	{
		auto it = sessionToPlayer_.find(sessionId);

		if (it == sessionToPlayer_.end())
			return nullptr;

		return GetPlayerByPlayerId(it->second);
	}

	// playerId -> Player
	Player* GetPlayerByPlayerId(int playerId)
	{
		auto it = playerMap_.find(playerId);

		if (it == playerMap_.end())
			return nullptr;

		return it->second;
	}

	// sessionId -> playerId
	int GetPlayerIdBySessionId(__int64 sessionId)
	{
		auto it = sessionToPlayer_.find(sessionId);

		if (it == sessionToPlayer_.end())
			return 0;

		return it->second;
	}

	// 접속 여부
	bool IsConnected(__int64 sessionId) const
	{
		return sessionToPlayer_.count(sessionId) != 0;
	}

	// playerId 존재 여부
	bool ContainsPlayer(int playerId) const
	{
		return playerMap_.count(playerId) != 0;
	}

	const std::unordered_map<int, Player*>& GetPlayers() const
	{
		return playerMap_;
	}

	std::unordered_map<int, Player*>& GetPlayers()
	{
		return playerMap_;
	}

};



// 클라이언트가 받을 대전 상대 플레이어
struct PlayerInfo
{
	int playerId_;
	std::string playerName_;
	int level_;

	PLAYER_STATE state_;

	PlayerInfo() = default;

	PlayerInfo(const Player& player)
		:playerId_(player.playerId_), playerName_(player.playerName_), level_(player.level_), state_(player.state_)
	{
	}
};
