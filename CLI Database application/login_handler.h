#pragma once
#include <crow.h>
#include <mysqlx/xdevapi.h>

class login_handler {


public:
	crow::response handleRequest(const crow::request& req);
private:

};