#pragma once

class InventoryRepository
{
private:
	Database* database_ = nullptr;
	int newInventoryId_;

public:
	void Initialize(Database* db)
	{
		database_ = db;

		// PK AUTO_INCREMENT 삭제. 게임 서버에서 id 할당할거임.
		auto stmt = database_->Prepare(
			"CREATE TABLE IF NOT EXISTS inventories("
			"inventory_id INT PRIMARY KEY,"
			"player_id INT NOT NULL,"
			"character_id INT NOT NULL,"
			"level INT NOT NULL DEFAULT 1,"
			//"attack INT NOT NULL,"
			//"hp INT NOT NULL,"
			//"stat_point INT NOT NULL DEFAULT 0,"
			"acquired_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,"
			"FOREIGN KEY (player_id) REFERENCES players(player_id)"
			");");

		stmt->execute();


		auto stmt2 = database_->Prepare(
			"SELECT IFNULL(MAX(inventory_id), 0) "
			"FROM inventories");

		std::unique_ptr<sql::ResultSet> result(stmt2->executeQuery());

		if (result->next())
		{
			newInventoryId_ = result->getInt(1) + 1;
		}
	}

	int GenerateInventoryId()
	{
		return newInventoryId_++;
	}

	void Create(int playerId, int characterId)
	{
		try
		{
			auto stmt = database_->Prepare(
				"INSERT INTO inventories(inventory_id, player_id, character_type_id) "
				"VALUES(?, ?, ?)");

			stmt->setInt(1, GenerateInventoryId());
			stmt->setInt(2, playerId);
			stmt->setInt(3, characterId);

			stmt->execute();
		}
		catch (sql::SQLException& e)
		{
			std::cout << "SQL Error: " << e.what() << std::endl;
			std::cout << "Error Code: " << e.getErrorCode() << std::endl;
			std::cout << "SQL State: " << e.getSQLState() << std::endl;
		}
	}

	RESPONSE_CODE SelectCharacters(int playerId, std::list<Character>& lst)
	{
		try
		{
			auto stmt = database_->Prepare(
				"SELECT inventory_id, player_id, character_id, level "
				"FROM inventories "
				"WHERE player_id = ?");

			stmt->setInt(1, playerId);

			std::unique_ptr<sql::ResultSet> result(stmt->executeQuery());

			lst.clear();

			while (result->next())
			{
				Character character;

				character.inventoryId_ = result->getInt("inventory_id");
				character.characterId_ = result->getInt("character_id");
				character.level_ = result->getInt("level");
				//character.attack_ = result->getInt("attack");
				//character.hp_ = result->getInt("hp");
				//character.statPoint_ = result->getInt("stat_point");

				lst.push_back(character);
			}

			if(lst.empty())
				return RESPONSE_CODE::NOT_FOUND;

			return RESPONSE_CODE::SUCCESS;
		}
		catch (sql::SQLException& e)
		{
			std::cout << "SQL Error: " << e.what() << std::endl;
			std::cout << "Error Code: " << e.getErrorCode() << std::endl;
			std::cout << "SQL State: " << e.getSQLState() << std::endl;

			return RESPONSE_CODE::DB_ERROR;
		}
	}

	void UpdateLevel(int inventoryId, int level)
	{
		try
		{
			auto stmt = database_->Prepare(
				"UPDATE inventories "
				"SET level = ? "
				"WHERE inventory_id = ?");

			stmt->setInt(1, level);
			stmt->setInt(2, inventoryId);

			stmt->execute();
		}
		catch (sql::SQLException& e)
		{
			std::cout << "SQL Error: " << e.what() << std::endl;
			std::cout << "Error Code: " << e.getErrorCode() << std::endl;
			std::cout << "SQL State: " << e.getSQLState() << std::endl;
		}
	}

	//void UpdateCharacterAttack(int inventoryId, int attack, int statPoint)
	//{
	//	try
	//	{
	//		auto stmt = database_->Prepare(
	//			"UPDATE inventories "
	//			"SET attack = ?, stat_point = ? "
	//			"WHERE inventory_id = ?");

	//		stmt->setInt(1, attack);
	//		stmt->setInt(2, statPoint);
	//		stmt->setInt(3, inventoryId);

	//		stmt->execute();
	//	}
	//	catch (sql::SQLException& e)
	//	{
	//		std::cout << "SQL Error: " << e.what() << std::endl;
	//		std::cout << "Error Code: " << e.getErrorCode() << std::endl;
	//		std::cout << "SQL State: " << e.getSQLState() << std::endl;
	//	}
	//}


	//void UpdateCharacterHp(int inventoryId, int hp, int statPoint)
	//{
	//	try
	//	{
	//		auto stmt = database_->Prepare(
	//			"UPDATE inventories "
	//			"SET hp = ?, stat_point = ? "
	//			"WHERE inventory_id = ?");

	//		stmt->setInt(1, hp);
	//		stmt->setInt(2, statPoint);
	//		stmt->setInt(3, inventoryId);

	//		stmt->execute();
	//	}
	//	catch (sql::SQLException& e)
	//	{
	//		std::cout << "SQL Error: " << e.what() << std::endl;
	//		std::cout << "Error Code: " << e.getErrorCode() << std::endl;
	//		std::cout << "SQL State: " << e.getSQLState() << std::endl;
	//	}
	//}
};
