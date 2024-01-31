#include "refresh_payments_handler.h"
#include "AuthorizationUtil.h"
#include "DatabaseConnector.h"
#include <rapidjson/allocators.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

refresh_payments_handler::refresh_payments_handler(DatabaseConnector& database) : database(database)
{

}

crow::response refresh_payments_handler::handleRequest(const crow::request& req, std::string& key)
{
	crow::json::wvalue response;
	try {
		auto jsonBody = crow::json::load(req.body);

		if (!jsonBody)
		{
			throw::std::invalid_argument("Invalid JSON");
		}

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

		std::string query = "SELECT * FROM transactions WHERE user_id = ?";


		sqlite3_stmt* statement;

		int result = sqlite3_prepare_v2(database.getDb(), query.c_str(), -1, &statement, nullptr);
		if (result == SQLITE_OK)
		{
			sqlite3_bind_text(statement, 1, user_id.c_str(), -1, SQLITE_STATIC);

			std::cerr << "Query bind, Now executing query. " << std::endl;
			std::cout << "Query: " << query << std::endl;

			if (database.executeQuery(statement))
			{
				sqlite3_reset(statement);
				int stepResult = sqlite3_step(statement);
				if (database.executeQuery(statement))
				{
					std::vector<crow::json::wvalue> transactionsArray;
					rapidjson::Document jsonDocument(rapidjson::kArrayType);


					while (sqlite3_step(statement) == SQLITE_ROW)
					{
						rapidjson::Value transaction(rapidjson::kObjectType);

						transaction.AddMember("ID", sqlite3_column_int(statement, 0), jsonDocument.GetAllocator());
						transaction.AddMember("price", sqlite3_column_double(statement, 1), jsonDocument.GetAllocator());
						transaction.AddMember("category", rapidjson::Value().SetString(reinterpret_cast<const char*>(sqlite3_column_text(statement, 2)), jsonDocument.GetAllocator()), jsonDocument.GetAllocator());
						transaction.AddMember("debit_credit", rapidjson::Value().SetString(reinterpret_cast<const char*>(sqlite3_column_text(statement, 3)), jsonDocument.GetAllocator()), jsonDocument.GetAllocator());
						transaction.AddMember("cleared", sqlite3_column_int(statement, 4), jsonDocument.GetAllocator());
						transaction.AddMember("date", rapidjson::Value().SetString(reinterpret_cast<const char*>(sqlite3_column_text(statement, 5)), jsonDocument.GetAllocator()), jsonDocument.GetAllocator());
						transaction.AddMember("user_id", sqlite3_column_double(statement, 6), jsonDocument.GetAllocator());

						jsonDocument.PushBack(transaction, jsonDocument.GetAllocator());
					}

					rapidjson::StringBuffer buffer;
					rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
					jsonDocument.Accept(writer);
					std::string jsonREsponse = buffer.GetString();

					sqlite3_finalize(statement);

					response["success"] = true;
					response["transactions"] = jsonREsponse;
				}

				else
				{
					response["success"] = false;
					response["error_message"] = "Invalid username and password.";
					std::cerr << "query failed";
				}
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