#include "login_handler.h"
#include <crow.h>
#include <mysqlx/xdevapi.h>
#include "AuthorizationUtil.h"
#include "DatabaseConnector.h"

login_handler::login_handler(DatabaseConnector& database) : database(database)
{

}

login_handler::~login_handler()
{

}

crow::response login_handler::handleRequest(const crow::request& req)
{
	crow::json::wvalue response;
	try {
		auto jsonBody = crow::json::load(req.body);

		if (!jsonBody)
		{
			throw::std::invalid_argument("Invalid JSON");
		}


		std::string username = jsonBody["username"].s();
		std::string password = jsonBody["password"].s();
		std::string query = "SELECT * FROM users WHERE username = ? AND password = ?";
		sqlite3_stmt* statement;
		if (sqlite3_prepare_v2(database.getDb(), query.c_str(), -1, &statement, nullptr) == SQLITE_OK)
		{
			sqlite3_bind_text(statement, 1, username.c_str(), -1, SQLITE_STATIC);
			sqlite3_bind_text(statement, 2, password.c_str(), -1, SQLITE_STATIC);

			if (database.executeQuery(statement))
			{
				response["sucess"] = true;
			}
			else
			{
				response["sucess"] = false;
				response["error_message"] = "Invalid username and passowrd.";
			}
			sqlite3_finalize(statement);
		}
		else
		{
			response["sucess"] = false;
			response["error_message"] = "Failed to prepare SQL statement.";

		}
	} 
	catch (const std::exception& e)
	{
		response["sucess"] = false;
		response["error_message"] = e.what();
	}
	return crow::response(response);
}