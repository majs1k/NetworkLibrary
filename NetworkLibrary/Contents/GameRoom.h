#pragma once
#include <vector>	

#include "Player.h"
#include "../RPC/RpcServerProxy.h"

class GameRoom
{
private:

    Player* player_[2];
    int commandType_[2];

    bool isGameOver_ = false;

public:

    GameRoom(Player* player1, Player* player2)
    {
        player_[0] = player1;
        player_[1] = player2;

        player1->room_ = this;
        player2->room_ = this;
    }

    bool IsGameOver()
    {
        return isGameOver_;
    }

    void StartGame()
    {
        // 게임 시작 패킷

        // TODO: 개선방법???
        PlayerInfo playerInfo0 = *(player_[0]);
        PlayerInfo playerInfo1 = *(player_[1]);


        player_[0]->rpcProxy_->ResStartGame(player_[0]->sessionId_, playerInfo1);
        player_[1]->rpcProxy_->ResStartGame(player_[1]->sessionId_, playerInfo0);
    }

    int GetPlayerIndex(Player* player)
    {
        if (player_[0] == player)
            return 0;

        if (player_[1] == player)
            return 1;

        return -1;
    }

    void ProcessCommand(Player* player, int commandType)
    {
        int idx = GetPlayerIndex(player);

        if (idx == -1)
            return;

        if (player_[idx]->state_ != PLAYER_STATE::GAME_WAITING)
            return;

        player_[idx]->state_ = PLAYER_STATE::GAME_COMMAND;
        commandType_[idx] = commandType;

        if (player_[0]->state_ == PLAYER_STATE::GAME_COMMAND && player_[1]->state_ == PLAYER_STATE::GAME_COMMAND)
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

        // 다음 턴
        player_[0]->state_ = PLAYER_STATE::GAME_WAITING;
        player_[1]->state_ = PLAYER_STATE::GAME_WAITING;

    }

    void EndGame()
    {
        // 종료 패킷
        
        // 보상 처리

        player_[0]->state_ = PLAYER_STATE::LOBBY;
        player_[1]->state_ = PLAYER_STATE::LOBBY;

        isGameOver_ = true;
    }
};


class GameRoomManager
{
private:

    std::vector<GameRoom*> rooms_;

public:

	void Create(Player* player1, Player* player2)
	{
		GameRoom* room = new GameRoom(player1, player2);

        rooms_.push_back(room);

		room->StartGame();
	}

	void Remove(GameRoom* room)
	{
        auto it = std::find(rooms_.begin(), rooms_.end(), room);

        if (it == rooms_.end())
            return;
            
        delete room;

        rooms_.erase(it);
	}
};
