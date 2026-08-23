#pragma once

class UserRepository
{
private:
	Database* database_;

public:
	void Init(Database* db)
	{
		database_ = db;

		auto stmt = database_->Prepare(
			"CREATE TABLE IF NOT EXISTS users("
			"user_id INT AUTO_INCREMENT PRIMARY KEY,"
			"login_id VARCHAR(30) UNIQUE,"
			"password VARCHAR(30),"
			"created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
			");");

		stmt->execute();
	}

	RESPONSE_CODE Register(const std::string& loginId, const std::string& password)
	{
		try
		{
			auto stmt = database_->Prepare(
				"INSERT INTO users(login_id, password) VALUES(?, ?)");

			stmt->setString(1, loginId);
			stmt->setString(2, password);

			stmt->execute();

			return RESPONSE_CODE::SUCCESS;
		}
		catch (sql::SQLException& e)
		{
			// login_id UNIQUE 에러
			if (e.getErrorCode() == 1062)
			{
				return RESPONSE_CODE::USER_REGISTER_ALREADY_EXISTS;
			}

			// 그 외 DB 오류
			std::cout << "SQL Error: " << e.what() << std::endl;
			std::cout << "Error Code: " << e.getErrorCode() << std::endl;
			std::cout << "SQL State: " << e.getSQLState() << std::endl;

			return RESPONSE_CODE::DB_ERROR;
		}
	}

	RESPONSE_CODE Login(const std::string& loginId, const std::string& password, int& userId)
	{
		try
		{
			auto stmt = database_->Prepare(
				"SELECT user_id, login_id, password "
				"FROM users "
				"WHERE login_id = ?");

			stmt->setString(1, loginId);

			std::unique_ptr<sql::ResultSet> result(stmt->executeQuery());

			// 아이디가 없음
			if (!result->next())
				return RESPONSE_CODE::USER_LOGIN_FAILED;

			//std::string loginId = result->getString("login_id");
			std::string pw = result->getString("password");

			// 비밀번호가 틀림
			if (pw != password)
				return RESPONSE_CODE::USER_LOGIN_FAILED;

			userId = result->getInt("user_id");

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
