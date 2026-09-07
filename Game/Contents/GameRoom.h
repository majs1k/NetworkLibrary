#pragma once
#include <vector>	

#include "RpcModule.h"
#include "Player.h"
#include "OmokGame.h"
#include "../Utils/TickController.h"

class GameRoom
{
private:

	int roomId_;

	Player* blackPlayer_ = nullptr;
	Player* whitePlayer_ = nullptr;

	int playerCount_ = 0;

	OmokGame omokGame_{};

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
		PlayerInfo blackInfo = blackPlayer_->ToInfo();
		PlayerInfo whiteInfo = whitePlayer_->ToInfo();

		// 흑인지 백인지 정보 전달 추가
		g_RpcProxy.ResStartMatch(blackPlayer_->sessionId_, STONE::BLACK, whiteInfo);
		g_RpcProxy.ResStartMatch(whitePlayer_->sessionId_, STONE::WHITE, blackInfo);
	}

	void PlaceStone(Player* player, int row, int col)
	{
		if (player == nullptr)
			return;

		// 해당 플레이어의 턴인지 확인
		if (!IsMyTurn(player))
			return;

		if (!omokGame_.PlaceStone(row, col))
			return;

		g_RpcProxy.ResPlaceStone(blackPlayer_->sessionId_, row, col);
		g_RpcProxy.ResPlaceStone(whitePlayer_->sessionId_, row, col);

		if (omokGame_.IsGameOver())
		{
			EndGame(player);
		}
	}

	Player* GetOpponentPlayer(Player* player) const
	{
		if (player == blackPlayer_)
			return whitePlayer_;
		else if (player == whitePlayer_)
			return blackPlayer_;
		else
			return nullptr;
	}

	void Update()
	{
		if (omokGame_.IsGameOver())
			return;

		// 초 단위로 타이머 업데이트
		omokGame_.UpdateTurnTimer(TickController::Instance().DeltaTime() / 1000);

		if (omokGame_.IsGameOver())
		{
			Player* loser = GetPlayer(omokGame_.GetTurn());

			EndGame(GetOpponentPlayer(loser));

			return;
		}
	}

	bool LeaveRoom(Player* player)
	{
		if (player == nullptr)
			return false;

		if (player != whitePlayer_ && player != blackPlayer_)
			return false;

		playerCount_--;

		g_RpcProxy.ResLeaveRoom(player->sessionId_, player->playerId_);

		// 두 명 다 나간 경우 방 삭제
		if (playerCount_ <= 0)
		{
			return true;
		}

		g_RpcProxy.ResLeaveRoom(GetOpponentPlayer(player)->sessionId_, player->playerId_);


		if (!omokGame_.IsGameOver())
		{
			// 나간 상대를 패배로 결과 처리
			EndGame(GetOpponentPlayer(player));
		}

		// 처리가 전부 끝난후 nullptr로 변경. 안 그러면 메모리 접근 에러 발생
		if (player == whitePlayer_)
			whitePlayer_ = nullptr;
		else if (player == blackPlayer_)
			blackPlayer_ = nullptr;

		player->gameRoom_ = nullptr;

		return false;
	}

	int GetRoomId() const
	{
		return roomId_;
	}

	bool IsEmpty() const
	{
		return blackPlayer_ == nullptr && whitePlayer_ == nullptr;
	}

	void BraodcastChat(Player* player, std::string& message)
	{
		g_RpcProxy.ResChat(blackPlayer_->sessionId_, player->playerId_, message);
		g_RpcProxy.ResChat(whitePlayer_->sessionId_, player->playerId_, message);
	}

private:

	void EndGame(Player* winner)
	{
		Player* loser = GetOpponentPlayer(winner);

		winner->money_ += 1000;
		winner->win_++;
		winner->rating_ += 10;

		loser->lose_++;
		loser->rating_ -= 10;

		// 승패 결과 패킷 보내기
		g_RpcProxy.ResGameResult(winner->sessionId_, omokGame_.GetWinner(), winner->rating_, loser->rating_, winner->money_);
		g_RpcProxy.ResGameResult(loser->sessionId_, omokGame_.GetWinner(), loser->rating_, winner->rating_, loser->money_);

		// 승패 결과 db 저장
		g_DbProxy.ReqGameResultDB(0, winner->playerId_, loser->playerId_);

		return;
	}

	bool IsMyTurn(Player* player) const
	{
		if (player->stone_ == omokGame_.GetTurn())
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

	void Update()
	{
		for (auto& r : rooms_)
		{
			r.second->Update();
		}
	}
};
