#pragma once
#include <crow.h>


class apiHandler
{
public:
	crow::response handleAPIRequest(const crow::response& req);
	void setKey(std::string key);
	std::string getKey();
private:
	std::string key;
};