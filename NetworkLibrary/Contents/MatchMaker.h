#pragma once
#include <vector>
#include "GameRoom.h"

class Player;
class GameRoom;


class MatchMaker
{
private:
	std::vector<Player*> matchQueue_;

	GameRoomManager* roomManager_;

public:

	void Initialize(GameRoomManager* roomManager)
	{
		roomManager_ = roomManager;
	}

	void Add(Player* player)
	{
		// 이미 큐에 등록되었다면 리턴
		if (std::find(matchQueue_.begin(), matchQueue_.end(), player) != matchQueue_.end())
			return;

		if (player->state_ != PLAYER_STATE::LOBBY)
			return;

		matchQueue_.push_back(player);

		// 2 초과는 될수 없음
		if (matchQueue_.size() == 2)
		{
			Player* player1 = matchQueue_[0];
			Player* player2 = matchQueue_[1];

			matchQueue_.clear();

			roomManager_->CreateRoom(player1, player2);
		}
	}

	void Remove(Player* player)
	{
		auto it = std::find(matchQueue_.begin(), matchQueue_.end(), player);

		if (it != matchQueue_.end())
			matchQueue_.erase(it);
	}
};
