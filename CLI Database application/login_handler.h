#pragma once
#include <crow.h>
#include <mysqlx/xdevapi.h>
#include "DatabaseConnector.h"

class login_handler {
public:
	login_handler(DatabaseConnector& database);
	~login_handler();
	crow::response handleRequest(const crow::request& req);
private:
	DatabaseConnector database;
};