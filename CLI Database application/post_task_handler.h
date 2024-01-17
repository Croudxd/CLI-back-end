#pragma once
#include "DatabaseConnector.h"
#include <crow.h>

class post_task_handler 
{
public:
	post_task_handler(DatabaseConnector& database);
	~post_task_handler();
	crow::response handleRequest(const crow::request& req, std::string& key);
private:
	DatabaseConnector database;
};