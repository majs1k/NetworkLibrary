#pragma once
#include <string>

struct Player
{
	__int64 sessionId_;

	int playerId_;
	std::string playerName_;
	int level_;
	int gold_;


	void Initialize(__int64 sessionId, int playerId, std::string& playerName, int level, int gold)
	{
		sessionId_ = sessionId;

		playerId_ = playerId;
		playerName_ = playerName;
		level_ = level;
		gold_ = gold;

	}
};
