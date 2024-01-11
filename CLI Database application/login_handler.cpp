#include "login_handler.h"
#include <crow.h>

crow::response login_handler::handleRequest(const crow::request& req)
{
	crow::json::wvalue response;
	try {
		auto jsonBody = crow::json::load(req.body);
		
		if (!jsonBody) 
		{
			throw::std::invalid_argument("Invalid JSON");
		}

		//
		std::string username = jsonBody["username"].s();
		std::string password = jsonBody["password"].s();
		//access json and check use logic
	}
	catch (const std::exception& e) {

	}

}
