#pragma once

class InventoryRepository
{
private:
	Database* database_ = nullptr;
	int newInventoryId_ = 1;

public:
	void Initialize(Database* db)
	{
		database_ = db;

		// PK AUTO_INCREMENT 삭제. 게임 서버에서 id 할당할거임.
		auto stmt = database_->Prepare(
			"CREATE TABLE IF NOT EXISTS inventories("
			"inventory_id INT PRIMARY KEY,"
			"player_id INT NOT NULL,"
			"icon_id INT NOT NULL,"
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

	void CreateIcon(int playerId, int inventoryId, int iconId)
	{
		try
		{
			auto stmt = database_->Prepare(
				"INSERT INTO inventories(inventory_id, player_id, icon_id) "
				"VALUES(?, ?, ?)");

			stmt->setInt(1, inventoryId);
			stmt->setInt(2, playerId);
			stmt->setInt(3, iconId);

			stmt->execute();
		}
		catch (sql::SQLException& e)
		{
			std::cout << "SQL Error: " << e.what() << std::endl;
			std::cout << "Error Code: " << e.getErrorCode() << std::endl;
			std::cout << "SQL State: " << e.getSQLState() << std::endl;
		}
	}

	void FindIconsByPlayerId(int playerId, std::vector<Icon>& icons)
	{
		try
		{
			auto stmt = database_->Prepare(
				"SELECT inventory_id, player_id, icon_id "
				"FROM inventories "
				"WHERE player_id = ?");

			stmt->setInt(1, playerId);

			std::unique_ptr<sql::ResultSet> result(stmt->executeQuery());

			icons.clear();

			while (result->next())
			{
				Icon icon;

				icon.inventoryId_ = result->getInt("inventory_id");
				icon.iconId_ = result->getInt("icon_id");

				icons.push_back(icon);
			}
		}
		catch (sql::SQLException& e)
		{
			std::cout << "SQL Error: " << e.what() << std::endl;
			std::cout << "Error Code: " << e.getErrorCode() << std::endl;
			std::cout << "SQL State: " << e.getSQLState() << std::endl;
		}
	}

	void DeleteIcon(int inventoryId)
	{
		try
		{
			auto stmt = database_->Prepare(
				"DELETE FROM inventories "
				"WHERE inventory_id = ?");

			stmt->setInt(1, inventoryId);

			stmt->execute();
		}
		catch (sql::SQLException& e)
		{
			std::cout << "SQL Error: " << e.what() << std::endl;
			std::cout << "Error Code: " << e.getErrorCode() << std::endl;
			std::cout << "SQL State: " << e.getSQLState() << std::endl;
		}
	}

};
