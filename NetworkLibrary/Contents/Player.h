#pragma once
#include <string>
#include <list>
#include <unordered_map>
#include "Character.h"

enum class PLAYER_STATE : short
{
	LOBBY,
	INGAME,
};

struct Player
{
	__int64 sessionId_;

	int playerId_;
	std::string playerName_;
	int level_;
	int gold_;

	PLAYER_STATE state_;

	std::list<Character> characterList_;



	void Initialize(__int64 sessionId, int playerId, std::string& playerName, int level, int gold)
	{
		sessionId_ = sessionId;

		playerId_ = playerId;
		playerName_ = playerName;
		level_ = level;
		gold_ = gold;

		state_ = PLAYER_STATE::LOBBY;
	}
};


class PlayerManager
{
private:
	std::unordered_map<int, Player*> playerMap_;
	int playerCount_ = 0;

	std::unordered_map<__int64, int> sessionToPlayer_;


public:
	//Player* GetPlayer(int playerId);
	//bool AddPlayer(Player* player);
	//void RemovePlayer(int playerId);
};

struct PlayerInfo
{
	int playerId_;
	std::string playerName_;
	int level_;

	PlayerInfo() = default;

	PlayerInfo(const Player& player)
		:playerId_(player.playerId_), playerName_(player.playerName_), level_(player.level_)
	{
	}
};
