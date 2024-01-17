#include "DatabaseConnector.h"
#include <sqlite3.h>
#include <string>
#include <iostream>

DatabaseConnector::DatabaseConnector(const std::string& dbname) : db(nullptr), dbname(dbname)
{

}
DatabaseConnector::~DatabaseConnector() 
{

}

bool DatabaseConnector::connect(sqlite3*& db)
{
	int result = sqlite3_open(dbname.c_str(), &db);

	if (result == SQLITE_OK)
	{
		std::cout << "Database connected...";
		this->db = db;
		//this->db = db;
		return true;
	}
	else
	{
		std::cerr << "Error connecting to database...";
		return false;
	}
}
bool DatabaseConnector::disconnect(sqlite3* db)
{
	int result = sqlite3_close(db);

	if (result == SQLITE_OK)
	{
		std::cout << "Database disconnected...";
		return true;
	}
	else
	{
		std::cout << "Error disconnecting database...";
		return false;
	}
}
bool DatabaseConnector::executeQuery(sqlite3_stmt* statement) const
{
	int result = sqlite3_step(statement);
	if (result == SQLITE_DONE || result == SQLITE_ROW)
	{
		std::cout << "Success executing Query" << std::endl;
		return true;
	}
	else
	{
		std::cerr << "Error executing query... " << result << std::endl;
		return false;
	}
	return true;
}

sqlite3* DatabaseConnector::getDb()
{
	return db;
}

bool DatabaseConnector::checkUserID(int user_id)
{
	std::string query = "SELECT ID FROM users WHERE ID = ?";
	sqlite3_stmt* statement;
	int result = sqlite3_prepare_v2(getDb(), query.c_str(), -1, &statement, nullptr);
	if (result == SQLITE_OK)
	{
		sqlite3_bind_int(statement, 1, user_id);

		if (executeQuery(statement))
		{
			sqlite3_reset(statement);
			int stepResult = sqlite3_step(statement);
			if (stepResult == SQLITE_ROW)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			std::cerr << "Error with executing statement: " << sqlite3_errmsg(getDb()) << std::endl;
			return false;
		}

	}
	else
	{
		std::cerr << "Error with preparing statement: " << sqlite3_errmsg(getDb()) << std::endl;
		return false;
	}
	sqlite3_finalize(statement);
}