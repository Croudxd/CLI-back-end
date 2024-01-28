#pragma once
#include <crow.h>
#include "DatabaseConnector.h"


class refresh_payments_handler
{
public:
	refresh_payments_handler(DatabaseConnector& database);
	crow::response handleRequest(const crow::request& req, std::string& key);
private:
	DatabaseConnector database;

};