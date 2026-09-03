#pragma once
#pragma comment(lib, "mysqlcppconn.lib")

#include <memory>
#include <string>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>


class Database
{
private:
	sql::mysql::MySQL_Driver* driver_ = nullptr;
	sql::Connection* connection_ = nullptr;

public:
	Database();
	~Database();

	bool Connect(std::string host, std::string user, std::string password, std::string schema);

	std::unique_ptr<sql::PreparedStatement> Prepare(const std::string& query);

	void BeginTransaction();
	void Commit();
	void Rollback();
};
