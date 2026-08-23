#pragma once


class PlayerRepository
{
private:
	Database* database_;

public:
	void Init(Database* db)
	{
		database_ = db;

		auto stmt = database_->Prepare(
			"CREATE TABLE IF NOT EXISTS players("
			"player_id INT AUTO_INCREMENT PRIMARY KEY,"
			"user_id INT UNIQUE,"
			"player_name VARCHAR(30) UNIQUE,"
			"level INT DEFAULT 1,"
			"gold INT DEFAULT 0,"
			"FOREIGN KEY (user_id) REFERENCES users(user_id)"
			");");

		stmt->execute();
	}

	RESPONSE_CODE InsertPlayer(int userId, const std::string& playerName)
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
				return RESPONSE_CODE::PLAYER_REGISTER_ALREADY_EXISTS;
			}

			// 그 외 DB 오류
			std::cout << "SQL Error: " << e.what() << std::endl;
			std::cout << "Error Code: " << e.getErrorCode() << std::endl;
			std::cout << "SQL State: " << e.getSQLState() << std::endl;

			return RESPONSE_CODE::DB_ERROR;
		}
	}

	RESPONSE_CODE SelectPlayer(int userId, Player& p)
	{
		try
		{
			auto stmt = database_->Prepare(
				"SELECT player_id, user_id, player_name, level, gold "
				"FROM players "
				"WHERE user_id = ?");

			stmt->setInt(1, userId);

			std::unique_ptr<sql::ResultSet> result(stmt->executeQuery());

			// 아이디가 없음
			if (!result->next())
			{
				return RESPONSE_CODE::PLAYER_NO_EXISTS;
			}

			p.playerId_ = result->getInt("player_id");
			p.playerName_ = result->getString("player_name");
			p.level_ = result->getInt("level");
			p.gold_ = result->getInt("gold");

			return RESPONSE_CODE::SUCCESS;
		}
		catch (sql::SQLException& e)
		{
			std::cout << "DB Error: " << e.what() << std::endl;
			std::cout << "Error Code: " << e.getErrorCode() << std::endl;
			std::cout << "SQL State: " << e.getSQLState() << std::endl;

			return RESPONSE_CODE::DB_ERROR;
		}
	}
};
