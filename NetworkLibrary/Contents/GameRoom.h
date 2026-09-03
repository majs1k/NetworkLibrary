#pragma once
#include <vector>	

#include "RpcModule.h"
#include "Player.h"
#include "OmokGame.h"

class GameRoom
{
private:

	int roomId_;

	Player* blackPlayer_ = nullptr;
	Player* whitePlayer_ = nullptr;

	int playerCount_ = 0;

	OmokGame game_{};

public:

	GameRoom(int roomId)
	{
		roomId_ = roomId;
	}

	bool EnterGameRoom(Player* player)
	{
		if (blackPlayer_ == nullptr)
		{
			blackPlayer_ = player;
			player->stone_ = STONE::BLACK;
			player->gameRoom_ = this;
			player->state_ = PLAYER_STATE::GAMEROOM;

			playerCount_++;

			return true;
		}
		else if (whitePlayer_ == nullptr)
		{
			whitePlayer_ = player;
			player->stone_ = STONE::WHITE;
			player->gameRoom_ = this;
			player->state_ = PLAYER_STATE::GAMEROOM;

			playerCount_++;

			return true;
		}
		else
			return false;
	}


	void StartGame()
	{
		// TODO: 개선방법???
		PlayerInfo blackInfo = *(blackPlayer_);
		PlayerInfo whiteInfo = *(whitePlayer_);


		// 흑인지 백인지 정보 전달 추가
		g_RpcProxy.ResStartMatch(blackPlayer_->sessionId_, STONE::BLACK, whiteInfo);
		g_RpcProxy.ResStartMatch(whitePlayer_->sessionId_, STONE::WHITE, blackInfo);
	}

	void PlaceStone(Player* player, int row, int col)
	{
		if (player == nullptr)
			return;


		// 해당 플레이어의 턴인지 확인 -> 이걸 어케?
		if (!IsMyTurn(player))
			return;


		if (!game_.PlaceStone(row, col))
			return;

		// TODO: 양쪽에 돌 위치 패킷보내기
		g_RpcProxy.ResPlaceStone(blackPlayer_->sessionId_, row, col);
		g_RpcProxy.ResPlaceStone(whitePlayer_->sessionId_, row, col);

		if (game_.IsGameOver())
		{
			EndGame(player);
		}
	}


private:

	void EndGame(Player* winner)
	{
		Player* loser = GetOpponentPlayer(winner);

		// TODO: 승패로 변경
		winner->money_ += 1000;
		winner->level_ += 1;


		// 승패 결과 패킷 보내기
		g_RpcProxy.ResGameResult(winner->sessionId_, game_.GetTurn(), winner->level_, winner->money_ + 1000);
		g_RpcProxy.ResGameResult(loser->sessionId_, game_.GetTurn(), loser->level_, loser->money_);

		// 승패 결과 db 저장
		g_DbProxy.ReqGameResultDB(0, winner->playerId_, loser->playerId_);

		return;
	}

	bool IsMyTurn(Player* player) const
	{
		if (player->stone_ == game_.GetTurn())
			return true;
		else
			return false;
	}

	Player* GetPlayer(STONE stone) const
	{
		if (blackPlayer_->stone_ == stone)
			return blackPlayer_;

		else if (whitePlayer_->stone_ == stone)
			return whitePlayer_;

		else
			return nullptr;
	}

public:

	Player* GetOpponentPlayer(Player* player) const
	{
		if (player == blackPlayer_)
			return whitePlayer_;
		else if (player == whitePlayer_)
			return blackPlayer_;
		else
			return nullptr;
	}


public:


	void Update()
	{
		if (game_.IsGameOver())
			return;

		// TODO:
		//auto deltaTime = ...
		//game_.UpdateTurnTimer(deltaTime);

		if (game_.IsGameOver())
		{
			Player* loser = GetPlayer(game_.GetTurn());

			EndGame(GetOpponentPlayer(loser));
			return;
		}
	}

	// TODO: 세션 종료시 방에 있었다면, 이 함수 호출
	bool LeaveRoom(Player* player)
	{
		if (player == nullptr)
			return false;

		if (player != whitePlayer_ && player != blackPlayer_)
			return false;

		playerCount_--;
		player->gameRoom_ = nullptr;

		g_RpcProxy.ResLeaveRoom(player->sessionId_, player->playerId_);

		// 두 명 다 나간 경우 방 삭제
		if (playerCount_ <= 0)
		{
			return true;
		}

		g_RpcProxy.ResLeaveRoom(GetOpponentPlayer(player)->sessionId_, player->playerId_);

		if (player == whitePlayer_)
			whitePlayer_ = nullptr;
		else if (player == blackPlayer_)
			blackPlayer_ = nullptr;

		if (!game_.IsGameOver())
		{
			// 나간 상대를 패배로 결과 처리
			EndGame(GetOpponentPlayer(player));
		}

		return false;
	}

public:

	int GetRoomId() const
	{
		return roomId_;
	}

	bool IsEmpty() const
	{
		return blackPlayer_ == nullptr && whitePlayer_ == nullptr;
	}

	void BraodcastChatting(Player* player, std::string& message)
	{
		g_RpcProxy.ResChat(blackPlayer_->sessionId_, player->playerId_, message);
		g_RpcProxy.ResChat(whitePlayer_->sessionId_, player->playerId_, message);
	}
};


//class GameRoomManager
//{
//private:
//
//	std::vector<GameRoom*> rooms_;
//
//public:
//
//	void Create(Player* player1, Player* player2)
//	{
//		GameRoom* room = new GameRoom(player1, player2);
//
//		rooms_.push_back(room);
//
//		room->StartGame();
//	}
//
//	void Remove(GameRoom* room)
//	{
//		auto it = std::find(rooms_.begin(), rooms_.end(), room);
//
//		if (it == rooms_.end())
//			return;
//
//		delete room;
//
//		rooms_.erase(it);
//	}
//};

class GameRoomManager
{
private:

	std::unordered_map<int, std::unique_ptr<GameRoom>> rooms_;

	int nextRoomId_ = 1;

	PlayerManager* playerManager_;

public:

	void SetPlayerManager(PlayerManager* playerManager)
	{
		playerManager_ = playerManager;
	}

	GameRoom* CreateRoom(Player* player1, Player* player2)
	{
		int roomId = nextRoomId_++;

		auto room = std::make_unique<GameRoom>(roomId);

		GameRoom* roomPtr = room.get();

		roomPtr->EnterGameRoom(player1);
		roomPtr->EnterGameRoom(player2);


		// 다른 로비 플레이어들에게 상태 변경 알림
		for (auto& p : playerManager_->GetPlayers())
		{
			if (p.second == player1 || p.second == player2)
				continue;
			if (p.second->state_ != PLAYER_STATE::LOBBY)
				continue;

			g_RpcProxy.ResChangePlayerState(p.second->sessionId_, player1->playerId_, player1->state_);
			g_RpcProxy.ResChangePlayerState(p.second->sessionId_, player2->playerId_, player2->state_);
		}

		roomPtr->StartGame();

		rooms_.emplace(roomId, std::move(room));

		return roomPtr;
	}

	void RemoveRoom(int roomId)
	{
		rooms_.erase(roomId);
	}
};
