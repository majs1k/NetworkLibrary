#include "DummyClient.h"

#include <iostream>
#include <cstring>

DummyClient::DummyClient(int dummyId)
	: dummyId_(dummyId)
{
	loginId_ = "dummy_" + std::to_string(dummyId_);
	password_ = "dummy1234";
	playerName_ = "Dummy_" + std::to_string(dummyId_);

	rpcProxy_.client_ = this;
}

bool DummyClient::Start(const std::wstring& ip, int port)
{
	if (!Connect(ip, port))
	{
		state_ = STATE::ERR;

		std::cout
			<< "[Dummy " << dummyId_
			<< "] Connect failed"
			<< std::endl;

		return false;
	}

	std::cout
		<< "[Dummy " << dummyId_
		<< "] Connected"
		<< std::endl;

	//rpcProxy_.ReqUserRegister(loginId_, password_);
	rpcProxy_.ReqUserLogin(loginId_, password_);

	return true;
}

int cnt;

void DummyClient::Update()
{
	if (IsFinished())
		return;

	ProcessNetwork();

	if (state_ == STATE::LOBBY)
	{
		std::string chat = u8"hello " + std::to_string(cnt++);
		rpcProxy_.ReqChat(chat);

		Sleep(30);
	}


	//if (state_ == STATE::GAME &&
	//    myTurn_ &&
	//    !gameOver_)
	//{
	//    TryMakeMove();
	//}
}

bool DummyClient::IsFinished() const
{
	return state_ == STATE::FINISHED ||
		state_ == STATE::ERR;
}

bool DummyClient::IsError() const
{
	return state_ == STATE::ERR;
}

int DummyClient::GetDummyId() const
{
	return dummyId_;
}

int DummyClient::GetUserId() const
{
	return userId_;
}

DummyClient::STATE DummyClient::GetState() const
{
	return state_;
}


// ============================================================
// LanClient
// ============================================================

void DummyClient::OnConnect()
{
}

void DummyClient::OnRelease()
{
	state_ = STATE::ERR;

	std::cout
		<< "[Dummy " << dummyId_
		<< "] Connection released"
		<< std::endl;
}

void DummyClient::OnRecv(Packet* packet)
{
	PacketProc(packet);

	delete packet;
}


// ============================================================
// Login
// ============================================================

bool DummyClient::ResUserRegister(RESPONSE_CODE code)
{
	std::cout
		<< "[Dummy " << dummyId_
		<< "] Register response"
		<< std::endl;

	// 이미 존재하는 계정이어도 로그인 시도
	state_ = STATE::LOGIN;

	rpcProxy_.ReqUserLogin(
		loginId_,
		password_);

	return true;
}

bool DummyClient::ResUserLogin(RESPONSE_CODE code, int userId)
{
	if (userId <= 0)
	{
		std::cout
			<< "[Dummy " << dummyId_
			<< "] Login failed"
			<< std::endl;

		state_ = STATE::ERR;
		return false;
	}

	userId_ = userId;

	std::cout
		<< "[Dummy " << dummyId_
		<< "] Login success"
		<< " UserId=" << userId_
		<< std::endl;

	rpcProxy_.ReqPlayerConnection(userId_);

	return true;
}


// ============================================================
// Player
// ============================================================

bool DummyClient::ResPlayerRegister(RESPONSE_CODE code)
{
	std::cout
		<< "[Dummy " << dummyId_
		<< "] Player register response"
		<< std::endl;

	state_ = STATE::PLAYER_CONNECTION;

	rpcProxy_.ReqPlayerConnection(userId_);

	return true;
}
bool DummyClient::ResPlayerProfile(Player& player)
{
	if (player.playerId_ == 0)
	{
		std::cout
			<< "[Dummy " << dummyId_
			<< "] Player does not exist"
			<< std::endl;

		rpcProxy_.ReqPlayerRegister(
			userId_,
			playerName_);

		return true;
	}

	if (player.playerId_ == -1)
	{
		std::cout
			<< "[Dummy " << dummyId_
			<< "] Player connection failed"
			<< std::endl;

		state_ = STATE::ERR;

		return false;
	}

	std::cout
		<< "[Dummy " << dummyId_
		<< "] Player connection success"
		<< std::endl;

	rpcProxy_.ReqEnterLobby();

	return true;
}


// ============================================================
// Lobby
// ============================================================

bool DummyClient::ResEnterLobby(PlayerInfo& player)
{
	state_ = STATE::LOBBY;

	std::cout
		<< "[Dummy " << dummyId_
		<< "] Enter lobby"
		<< std::endl;

	//rpcProxy_.ReqStartMatch();

	return true;
}

bool DummyClient::ResLeaveLobby(int playerId)
{
	return true;
}


// ============================================================
// Match
// ============================================================

bool DummyClient::ResStartMatch(STONE stone, PlayerInfo& opponent)
{
	if (state_ != STATE::MATCHING)
		return true;

	ResetGame();

	myStone_ = stone;
	myTurn_ = (myStone_ == STONE::BLACK);

	state_ = STATE::GAME;

	std::cout
		<< "[Dummy " << dummyId_
		<< "] Match started Stone="
		<< (myStone_ == STONE::BLACK
			? "BLACK"
			: "WHITE")
		<< std::endl;

	return true;
}


// ============================================================
// Game
// ============================================================

void DummyClient::ResetGame()
{
	std::memset(
		board_,
		false,
		sizeof(board_));

	myStone_ = STONE::NONE;

	myTurn_ = false;
	gameOver_ = false;

	moveCount_ = 0;
	nextMoveIndex_ = 0;
}

void DummyClient::TryMakeMove()
{
	if (!myTurn_ ||
		gameOver_)
	{
		return;
	}

	auto [row, col] = GetNextMove();

	if (row < 0 || col < 0)
	{
		state_ = STATE::ERR;
		return;
	}

	board_[row][col] = true;

	++moveCount_;

	myTurn_ = false;

	rpcProxy_.ReqPlaceStone(
		row,
		col);
}

std::pair<short, short>
DummyClient::GetNextMove()
{
	constexpr int BOARD_SIZE = 15;

	for (int i = nextMoveIndex_;
		i < BOARD_SIZE * BOARD_SIZE;
		++i)
	{
		int row = i / BOARD_SIZE;
		int col = i % BOARD_SIZE;

		if (!board_[row][col])
		{
			nextMoveIndex_ = i + 1;

			return {
				static_cast<short>(row),
				static_cast<short>(col)
			};
		}
	}

	return { -1, -1 };
}

bool DummyClient::ResPlaceStone(
	short row,
	short col)
{
	if (row < 0 || row >= 15 ||
		col < 0 || col >= 15)
	{
		state_ = STATE::ERR;
		return false;
	}

	board_[row][col] = true;

	++moveCount_;

	myTurn_ = !myTurn_;

	return true;
}


// ============================================================
// Game Result
// ============================================================

bool DummyClient::ResGameResult(
	STONE winner,
	int myRating,
	int opponentRating,
	int myMoney)
{
	gameOver_ = true;
	myTurn_ = false;

	state_ = STATE::GAME_OVER;

	std::cout
		<< "[Dummy " << dummyId_
		<< "] Game finished ";

	if (winner == STONE::NONE)
	{
		std::cout << "DRAW";
	}
	else if (winner == myStone_)
	{
		std::cout << "WIN";
	}
	else
	{
		std::cout << "LOSE";
	}

	std::cout
		<< " MoveCount=" << moveCount_
		<< std::endl;

	rpcProxy_.ReqLeaveRoom();

	return true;
}


// ============================================================
// Leave Room
// ============================================================

bool DummyClient::ResLeaveRoom(int playerId)
{
	if (playerId == userId_)
	{
		state_ = STATE::FINISHED;

		std::cout
			<< "[Dummy " << dummyId_
			<< "] Finished"
			<< std::endl;

		return true;
	}

	return true;
}
