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
		std::cout << username << std::endl;
		std::string password = jsonBody["password"].s();
		std::cout << password << std::endl;
		std::string query = "SELECT * FROM users WHERE username = ? AND password = ?";
		sqlite3_stmt* statement;
		int result = sqlite3_prepare_v2(database.getDb(), query.c_str(), -1, &statement, nullptr);
		std::cout << result << sqlite3_errmsg(database.getDb()) << std::endl;
		if (result == SQLITE_OK)
		{
			sqlite3_bind_text(statement, 1, username.c_str(), -1, SQLITE_STATIC);
			sqlite3_bind_text(statement, 2, password.c_str(), -1, SQLITE_STATIC);
			const char* sql = sqlite3_sql(statement);
			std::cout << "SQL Statement: " << sql << std::endl;

			if (database.executeQuery(statement))
			{
				response["success"] = true;
			}
			else
			{
				response["success"] = false;
				response["error_message"] = "Invalid username and passowrd.";
			}
			sqlite3_finalize(statement);
		}
		else
		{
			const char* errorMessage = sqlite3_errmsg(database.getDb());
			std::cerr << "SQLite error: " << errorMessage << std::endl;
			response["success"] = false;
			response["error_message"] = "Failed to prepare SQL statement.";

		}
	} 
	catch (const std::exception& e)
	{
		response["success"] = false;
		response["error_message"] = e.what();
	}

	return crow::response(response);
}