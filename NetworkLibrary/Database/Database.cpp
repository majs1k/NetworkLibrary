#include "Database.h"
#include <iostream>

Database::Database()
{
}

Database::~Database()
{
	delete connection_;
}

bool Database::Connect()
{
	try
	{
		driver_ = sql::mysql::get_mysql_driver_instance();

		connection_ = driver_->connect(DB_HOST, DB_USER, DB_PASSWORD);

		connection_->setSchema(DB_SCHEMA);

		return true;
	}
	catch (sql::SQLException& e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}
}

std::unique_ptr<sql::PreparedStatement> Database::Prepare(const std::string& query)
{
	return std::unique_ptr<sql::PreparedStatement>(connection_->prepareStatement(query));
}

void Database::BeginTransaction()
{
	connection_->setAutoCommit(false);
}

void Database::Commit()
{
	connection_->commit();
	connection_->setAutoCommit(true);
}

void Database::Rollback()
{
	connection_->rollback();
	connection_->setAutoCommit(true);
}
