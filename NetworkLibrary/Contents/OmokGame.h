#pragma once


class OmokGame
{
public:
	static constexpr int BOARD_SIZE = 15;
	static constexpr int MAX_TURN_TIME = 30;

	enum class Stone
	{
		None ,
		Black ,
		White ,
	};

	class MyStone
	{
		Stone stone_;
		int sequence_;
	};

public:
	OmokGame()
	{
		StartGame();
	}

	bool PlaceStone(int row, int col)
	{
		if (isGameOver_)
			return false;

		if (!IsValidPosition(row, col))
			return false;

		if (board_[row][col] != Stone::None)
			return false;

		Stone stone = currentTurn_;
		board_[row][col] = stone;

		if (CheckWin(row, col, stone))
		{
			isGameOver_ = true;
			winner_ = stone;

			return true;
		}

		currentTurn_ = GetOpponentStone(currentTurn_);
		turnTime_ = MAX_TURN_TIME;

		return true;
	}

	void StartGame()
	{
		for (auto& row : board_)
			for (auto& elem : row)
				elem = Stone::None;

		currentTurn_ = Stone::Black;
		isGameOver_ = false;
		winner_ = Stone::None;
		turnTime_ = MAX_TURN_TIME;
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
		winner_ = GetOpponentStone(currentTurn_);
	}

	bool IsGameOver() const
	{
		return isGameOver_;
	}

	Stone GetStone(int row, int col) const
	{
		if (!IsValidPosition(row, col))
			return Stone::None;

		return board_[row][col];
	}

	Stone GetCurrentTurn() const
	{
		return currentTurn_;
	}

	Stone GetWinner() const
	{
		return winner_;
	}

	int GetTurnTime() const
	{
		return static_cast<int>(turnTime_ + 1);
	}


private:
	bool CheckWin(int row, int col, Stone stone) const
	{
		static constexpr int directions[4][2] =
		{
			{ 1, 0 },
			{ 0, 1 },
			{ 1, 1 },
			{ 1, -1 }
		};

		for (const auto& direction : directions)
		{
			int count = 1;

			for (int i = 1; i < 5; ++i)
			{
				int nextRow = row + direction[0] * i;
				int nextCol = col + direction[1] * i;

				if (!IsValidPosition(nextRow, nextCol))
					break;

				if (board_[nextRow][nextCol] != stone)
					break;

				count++;
			}

			for (int i = 1; i < 5; ++i)
			{
				int nextRow = row - direction[0] * i;
				int nextCol = col - direction[1] * i;

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

	Stone GetOpponentStone(Stone stone) const
	{
		return stone == Stone::Black ? Stone::White : Stone::Black;
	}

private:
	Stone board_[BOARD_SIZE][BOARD_SIZE];

	Stone currentTurn_ = Stone::Black;
	Stone winner_ = Stone::None;
	bool isGameOver_ = false;
	float turnTime_ = MAX_TURN_TIME;

	//bool sequenceFlag = false;
};
