#pragma once
#include <queue>
#include "GameRoom.h"

class Player;
class GameRoom;


// TODO: 4명이상 매칭 시스템으로 업데이트
class MatchMaker
{
private:
	std::queue<Player*> matchQueue_;

	GameRoomManager* roomManager_;

public:

	MatchMaker(GameRoomManager* roomManager)
		: roomManager_(roomManager)
	{
	}

	void Enqueue(Player* player)
	{
		matchQueue_.push(player);

		if (matchQueue_.size() >= 2)
		{
			Player* player1 = matchQueue_.front();

			matchQueue_.pop();

			Player* player2 = matchQueue_.front();

			matchQueue_.pop();

			roomManager_->CreateGameRoom(player1, player2);
		}
	}
};
