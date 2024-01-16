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
		std::cout << "Success executing Query" << statement << std::endl;
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