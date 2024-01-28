#include "post_payment_handler.h"
#include <crow.h>
#include "AuthorizationUtil.h"
#include "DatabaseConnector.h"

post_payment_handler::post_payment_handler(DatabaseConnector& database) : database(database)
{

}


post_payment_handler::~post_payment_handler()
{

}

crow::response post_payment_handler::handleRequest(const crow::request& req, std::string& key)
{
	crow::json::wvalue response;
	try {
		auto jsonBody = crow::json::load(req.body);

		if (!jsonBody)
		{
			throw::std::invalid_argument("Invalid JSON");
		}

		int payment_id = jsonBody["payment_id"].i();
		int price = jsonBody["price"].i();
		std::string category = jsonBody["category"].s();
		int cleared = jsonBody["cleared"].b();
		std::string debitCredit = jsonBody["debitCredit"].s();
		std::string date = jsonBody["date"].s();
		std::string userId = jsonBody["user_id"].s();

		std::string access_key = req.get_header_value("access_token");

		std::string user_id = AuthorizationUtil::decodeAuthorizationKey(access_key, key);

		if (!database.checkUserID(stoi(user_id)))
		{
			response["success"] = false;
			response["error_message"] = "No user id associated with token.";
			std::cerr << "No user id associated with token" << std::endl;
			return crow::response(response);
			exit;
		}

		std::string query = "INSERT INTO transactions (ID, price, category, debit_credit, cleared, date, user_id) VALUES (?, ?, ?, ?, ?, ?, ?)"; //Get query ready.


		sqlite3_stmt* statement;

		int result = sqlite3_prepare_v2(database.getDb(), query.c_str(), -1, &statement, nullptr);
		if (result == SQLITE_OK)
		{
			sqlite3_bind_int(statement, 1, payment_id);
			sqlite3_bind_int(statement, 2, price);
			sqlite3_bind_text(statement, 3, category.c_str(), -1, SQLITE_STATIC);
			sqlite3_bind_int(statement, 4, cleared);
			sqlite3_bind_text(statement, 5, debitCredit.c_str(), -1, SQLITE_STATIC);
			sqlite3_bind_text(statement, 6, date.c_str(), -1, SQLITE_STATIC);
			sqlite3_bind_text(statement, 7, user_id.c_str(), -1, SQLITE_STATIC);

			std::cerr << "Query bind, Now executing query. " << std::endl;
			std::cout << "Query: " << query << std::endl;

			if (database.executeQuery(statement))
			{
				/*sqlite3_reset(statement);
				int stepResult = sqlite3_step(statement);*/
				std::cout << "query success";
				response["success"] = true;
			}
			else
			{
				response["success"] = false;
				response["error_message"] = "Invalid username and passowrd.";
				std::cerr << "query failed";
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
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return crow::response(response);
}