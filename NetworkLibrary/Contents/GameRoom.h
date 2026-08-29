#pragma once
#include <vector>	

#include "Player.h"

class MyServer;
class Player;


class GameRoom
{
private:
    struct GamePlayer
    {
        Player* player = nullptr;
        bool isCommand_ = false;
        int commandType_ = 0;
    };

    MyServer* server_;

    GamePlayer player1_;
    GamePlayer player2_;

public:
    GameRoom(MyServer* server, Player* player1, Player* player2)
        : server_(server)
    {
        player1_.player = player1;
        player2_.player = player2;
    }

    void StartGame()
    {
        // 게임 시작 패킷
    }

    void ProcessCommand(Player* player, int commandType)
    {
        GamePlayer* gamePlayer = nullptr;

        if (player1_.player == player)
            gamePlayer = &player1_;
        else if (player2_.player == player)
            gamePlayer = &player2_;
        else
            return;

        if (gamePlayer->isCommand_)
            return;

        gamePlayer->isCommand_ = true;
        gamePlayer->commandType_ = commandType;

        if (player1_.isCommand_ && player2_.isCommand_)
        {
            ProcessTurn();
        }
    }

    void ProcessTurn()
    {
        // 공격 계산

        // 결과 패킷

        if (1/* 체력 0 */)
        {
            EndGame();
            return;
        }

        player1_.isCommand_ = false;
        player2_.isCommand_ = false;

        // 다음 턴
    }

    void EndGame()
    {
        // 종료 패킷
        // 보상 처리
    }

    Player* GetPlayer(int index)
    {
        if (index == 0)
            return player1_.player;

        if (index == 1)
            return player2_.player;

        return nullptr;
    }
};


class GameRoomManager
{
private:
	
	MyServer* server_;

	//std::vector<GameRoom*> rooms_;

	std::unordered_map<int, GameRoom*> rooms_;

public:

	GameRoomManager(MyServer* server)
		: server_(server)
	{
	}

	void CreateGameRoom(Player* player1, Player* player2)
	{
		GameRoom* gm = new GameRoom(server_, player1, player2);

		rooms_.insert({ player1->playerId_, gm });
		rooms_.insert({ player2->playerId_, gm });


		gm->StartGame();
	}

	// MyServer에서는 이 함수만 호출
	void ProcessCommand(Player* player, int commandType)
	{
		GameRoom* gm = FindRoomByPlayerId(player->playerId_);

		if (gm == nullptr)
			return;

		gm->ProcessCommand(player, commandType);
	}

	GameRoom* FindRoomByPlayerId(int playerId)
	{
		auto it = rooms_.find(playerId);

		if (it == rooms_.end())
			return nullptr;

		return it->second;
	}

	void RemoveGameRoom(GameRoom* room)
	{
		rooms_.erase(room->GetPlayer(0)->playerId_);
		rooms_.erase(room->GetPlayer(1)->playerId_);

		delete room;
	}

};
