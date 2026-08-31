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
			"level INT NOT NULL DEFAULT 1,"
			"gold INT NOT NULL DEFAULT 10000,"
			"equipped_character_id INT NOT NULL DEFAULT 0,"
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
				"SELECT player_id, player_name, level, gold, equipped_character_id "
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
			p.level_ = result->getInt("level");
			p.gold_ = result->getInt("gold");
			p.equippedInvenId_ = result->getInt("equipped_character_id");
		}
		catch (sql::SQLException& e)
		{
			std::cout << "DB Error: " << e.what() << std::endl;
			std::cout << "Error Code: " << e.getErrorCode() << std::endl;
			std::cout << "SQL State: " << e.getSQLState() << std::endl;
		}
	}

	void UpdateLevel(int playerId, int level)
	{
		try
		{
			auto stmt = database_->Prepare(
				"UPDATE players "
				"SET level = ? "
				"WHERE player_id = ?");

			stmt->setInt(1, level);
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

	void UpdateGold(int playerId, int gold)
	{
		try
		{
			auto stmt = database_->Prepare(
				"UPDATE players "
				"SET gold = ? "
				"WHERE player_id = ?");

			stmt->setInt(1, gold);
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

	void UpdateEquipment(int playerId, int characterId)
	{
		try
		{
			auto stmt = database_->Prepare(
				"UPDATE players "
				"SET equipped_character_id = ? "
				"WHERE player_id = ?");

			stmt->setInt(1, characterId);
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
