#include "post_task_handler.h"
#include "DatabaseConnector.h"
#include <crow.h>
#include "AuthorizationUtil.h"

post_task_handler::post_task_handler(DatabaseConnector& database) : database(database)
{

}
post_task_handler::~post_task_handler()
{

}

crow::response post_task_handler::handleRequest(const crow::request& req, std::string& key)
{
	crow::json::wvalue response;
	try {
		auto jsonBody = crow::json::load(req.body);

		if (!jsonBody)
		{
			throw::std::invalid_argument("Invalid JSON");
		}

		std::string task_name = jsonBody["task_name"].s();
		std::string task_description = jsonBody["task_description"].s();
		std::string access_key = req.get_header_value("access_token");

		std::string user_id = AuthorizationUtil::decodeAuthorizationKey(access_key, key);

		if (!database.checkUserID(stoi(user_id)))
		{
			response["success"] = false;
			response["error_message"] = "No user id associated with token.";
			return crow::response(response);
		}

		std::string query = "INSERT INTO tasks (task_name, task_description, user_id) VALUES (?, ?, ?)"; //Get query ready.
		

		sqlite3_stmt* statement;

		int result = sqlite3_prepare_v2(database.getDb(), query.c_str(), -1, &statement, nullptr);
		if (result == SQLITE_OK)
		{
			sqlite3_bind_text(statement, 1, task_name.c_str(), -1, SQLITE_STATIC);
			sqlite3_bind_text(statement, 2, task_description.c_str(), -1, SQLITE_STATIC);
			sqlite3_bind_text(statement, 3, user_id.c_str(), -1, SQLITE_STATIC);


			if (database.executeQuery(statement))
			{
				sqlite3_reset(statement);
				int stepResult = sqlite3_step(statement);
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
		response["error_message"] = typeid(e).name() + std::string(": ") + e.what();
	}

	return crow::response(response);
}
