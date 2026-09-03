#pragma once


class PlayerRepository
{
private:
	Database* database_;

public:
	void Initialize(Database* db)
	{
		database_ = db;

		// TODO: 외래키 삭제
		auto stmt = database_->Prepare(
			"CREATE TABLE IF NOT EXISTS players("
			"player_id INT AUTO_INCREMENT PRIMARY KEY,"
			"user_id INT NOT NULL UNIQUE,"
			"player_name VARCHAR(30) NOT NULL UNIQUE,"
			"money INT NOT NULL DEFAULT 10000,"
			"win INT NOT NULL DEFAULT 0,"
			"lose INT NOT NULL DEFAULT 0,"
			"rating INT NOT NULL DEFAULT 1000,"
			"profile_icon_id INT NOT NULL DEFAULT 0,"
			"FOREIGN KEY (user_id) REFERENCES users(user_id)"
			");");

		stmt->execute();
	}

	RESPONSE_CODE CreatePlayer(int userId, const std::string& playerName)
	{
		try
		{
			auto stmt = database_->Prepare(
				"INSERT INTO players(user_id, player_name) VALUES(?, ?)");

			stmt->setInt(1, userId);
			stmt->setString(2, playerName);

			stmt->execute();

			return RESPONSE_CODE::SUCCESS;
		}
		catch (sql::SQLException& e)
		{
			// user_id, player_name UNIQUE 제약조건 위반
			if (e.getErrorCode() == 1062)
			{
				return RESPONSE_CODE::ALREADY_EXISTS;
			}

			// 그 외 DB 오류
			std::cout << "SQL Error: " << e.what() << std::endl;
			std::cout << "Error Code: " << e.getErrorCode() << std::endl;
			std::cout << "SQL State: " << e.getSQLState() << std::endl;

			return RESPONSE_CODE::DB_ERROR;
		}
	}

	void FindPlayerByUserId(int userId, Player& p)
	{
		try
		{
			auto stmt = database_->Prepare(
				"SELECT player_id, player_name, win, lose, rating, money, profile_icon_id "
				"FROM players "
				"WHERE user_id = ?");

			stmt->setInt(1, userId);

			std::unique_ptr<sql::ResultSet> result(stmt->executeQuery());

			// 플레이어 생성되지 않았음
			if (!result->next())
			{
				return;
			}

			p.playerId_ = result->getInt("player_id");
			p.playerName_ = result->getString("player_name");
			p.win_ = result->getInt("win");
			p.lose_ = result->getInt("lose");
			p.rating_ = result->getInt("rating");
			p.money_ = result->getInt("money");
			p.equippedInvenId_ = result->getInt("profile_icon_id");
		}
		catch (sql::SQLException& e)
		{
			std::cout << "DB Error: " << e.what() << std::endl;
			std::cout << "Error Code: " << e.getErrorCode() << std::endl;
			std::cout << "SQL State: " << e.getSQLState() << std::endl;
		}
	}

	void UpdateWin(int playerId, int win)
	{
		try
		{
			auto stmt = database_->Prepare(
				"UPDATE players "
				"SET win = ? "
				"WHERE player_id = ?");

			stmt->setInt(1, win);
			stmt->setInt(2, playerId);

			stmt->execute();
		}
		catch (sql::SQLException& e)
		{
			std::cout << "DB Error: " << e.what() << std::endl;
			std::cout << "Error Code: " << e.getErrorCode() << std::endl;
			std::cout << "SQL State: " << e.getSQLState() << std::endl;
		}
	}

	void UpdateLose(int playerId, int lose)
	{
		try
		{
			auto stmt = database_->Prepare(
				"UPDATE players "
				"SET lose = ? "
				"WHERE player_id = ?");

			stmt->setInt(1, lose);
			stmt->setInt(2, playerId);

			stmt->execute();
		}
		catch (sql::SQLException& e)
		{
			std::cout << "DB Error: " << e.what() << std::endl;
			std::cout << "Error Code: " << e.getErrorCode() << std::endl;
			std::cout << "SQL State: " << e.getSQLState() << std::endl;
		}
	}

	void UpdateRating(int playerId, int rating)
	{
		try
		{
			auto stmt = database_->Prepare(
				"UPDATE players "
				"SET rating = ? "
				"WHERE player_id = ?");

			stmt->setInt(1, rating);
			stmt->setInt(2, playerId);

			stmt->execute();
		}
		catch (sql::SQLException& e)
		{
			std::cout << "DB Error: " << e.what() << std::endl;
			std::cout << "Error Code: " << e.getErrorCode() << std::endl;
			std::cout << "SQL State: " << e.getSQLState() << std::endl;
		}
	}

	void UpdateMoney(int playerId, int money)
	{
		try
		{
			auto stmt = database_->Prepare(
				"UPDATE players "
				"SET money = ? "
				"WHERE player_id = ?");

			stmt->setInt(1, money);
			stmt->setInt(2, playerId);

			stmt->execute();
		}
		catch (sql::SQLException& e)
		{
			std::cout << "DB Error: " << e.what() << std::endl;
			std::cout << "Error Code: " << e.getErrorCode() << std::endl;
			std::cout << "SQL State: " << e.getSQLState() << std::endl;
		}
	}

	void UpdateProfileIcon(int playerId, int iconId)
	{
		try
		{
			auto stmt = database_->Prepare(
				"UPDATE players "
				"SET profile_icon_id = ? "
				"WHERE player_id = ?");

			stmt->setInt(1, iconId);
			stmt->setInt(2, playerId);

			stmt->execute();
		}
		catch (sql::SQLException& e)
		{
			std::cout << "DB Error: " << e.what() << std::endl;
			std::cout << "Error Code: " << e.getErrorCode() << std::endl;
			std::cout << "SQL State: " << e.getSQLState() << std::endl;
		}
	}
};
