#pragma once
#include <queue>

// TODO: 4명이상 매칭 시스템으로 업데이트
class MatchMaker
{
private:
	std::queue<int> matchQueue_;

public:
	int Enqueue(int playerId)
	{
		if (matchQueue_.empty())
		{
			matchQueue_.push(playerId);

			return 0;
		}

		int otherPlayerId = matchQueue_.front();

		matchQueue_.pop();

		return otherPlayerId;
	}
};
