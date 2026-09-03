#pragma once

enum class STONE
{
	NONE,
	BLACK,
	WHITE,
};

class MYSTONE
{
	STONE stone_;
	int sequence_;
};

class OmokGame
{
public:
	// 보드의 네모(cell) 칸은 줄 당 14개
	// 돌을 놓을 수 있는 교차점은 줄 당 15개
	static constexpr int BOARD_SIZE = 15;
	static constexpr int MAX_TURN_TIME = 30;

public:

	void Initialize()
	{
		for (auto& row : board_)
		{
			for (auto& elem : row)
			{
				elem = STONE::NONE;
			}
		}

		turn_ = STONE::BLACK;
		isGameOver_ = false;
		winner_ = STONE::NONE;
		turnTime_ = MAX_TURN_TIME;
	}

	bool PlaceStone(int row, int col)
	{
		if (isGameOver_)
			return false;

		if (!IsValidPosition(row, col))
			return false;

		if (board_[row][col] != STONE::NONE)
			return false;

		STONE stone = turn_;
		board_[row][col] = stone;

		// 이 로직을 클라이언트에서는 분리??
		if (IsOmok(row, col))
		{
			isGameOver_ = true;
			winner_ = stone;

			return true;
		}

		turn_ = ChangeTurn(turn_);
		turnTime_ = MAX_TURN_TIME;

		return true;
	}

	void UpdateTurnTimer(float deltaTime)
	{
		if (isGameOver_)
			return;

		turnTime_ -= deltaTime;

		if (turnTime_ > 0.0f)
			return;

		turnTime_ = 0.0f;
		isGameOver_ = true;
		winner_ = ChangeTurn(turn_);
	}

	bool IsGameOver() const
	{
		return isGameOver_;
	}

	STONE GetStone(int row, int col) const
	{
		if (!IsValidPosition(row, col))
			return STONE::NONE;

		return board_[row][col];
	}

	STONE GetTurn() const
	{
		return turn_;
	}

	STONE GetWinner() const
	{
		return winner_;
	}

	int GetTurnTime() const
	{
		return static_cast<int>(turnTime_ + 1);
	}

	// 클라이언트 호출용
	void SetWinner(STONE stone)
	{
		isGameOver_ = true;
		winner_ = stone;
	}


private:

	// 가로 세로 대각 대각
	int dx[4] = { 1,0,1,1 };
	int dy[4] = { 0, 1, 1, -1 };


	bool IsOmok(int row, int col) const
	{
		STONE stone = board_[row][col];

		for (int i = 0; i < 4; i++)
		{
			int count = 1;

			// 정방향
			for (int j = 1; j < 5; j++)
			{
				int nextRow = row + dx[i] * j;
				int nextCol = col + dy[i] * j;

				if (!IsValidPosition(nextRow, nextCol))
					break;

				if (board_[nextRow][nextCol] != stone)
					break;

				count++;
			}

			// 역방향
			for (int j = 1; j < 5; j++)
			{
				int nextRow = row - dx[i] * j;
				int nextCol = col - dy[i] * j;

				if (!IsValidPosition(nextRow, nextCol))
					break;

				if (board_[nextRow][nextCol] != stone)
					break;

				count++;
			}

			if (count >= 5)
				return true;
		}

		return false;
	}

	bool IsValidPosition(int row, int col) const
	{
		return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
	}

	STONE ChangeTurn(STONE stone) const
	{
		return stone == STONE::BLACK ? STONE::WHITE : STONE::BLACK;
	}

private:

	STONE board_[BOARD_SIZE][BOARD_SIZE];

	STONE turn_ = STONE::BLACK;
	STONE winner_ = STONE::NONE;


	bool isGameOver_ = false;
	float turnTime_ = MAX_TURN_TIME;

	//bool sequenceFlag = false;
};
