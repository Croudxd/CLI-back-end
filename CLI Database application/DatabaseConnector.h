#pragma once
#include <sqlite3.h>
#include <string>


class DatabaseConnector
{
public:
	DatabaseConnector(const std::string& dbname);
	~DatabaseConnector();

	bool connect(sqlite3*& db);
	bool disconnect(sqlite3* db);
	bool executeQuery(sqlite3_stmt* statement) const;
	sqlite3* getDb();

private:
	std::string dbname;
	sqlite3* db;
};	