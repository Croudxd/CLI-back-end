#pragma once
#include "DatabaseConnector.h"
#include <crow.h>

class post_payment_handler
{
public:
	post_payment_handler(DatabaseConnector& database);
	~post_payment_handler();
	crow::response handleRequest(const crow::request& req, std::string& key);

private:
	DatabaseConnector database;

};