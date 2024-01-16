#include <iostream>
#include <crow.h>
#include "login_handler.h"
#include "api_handler.h"
#include "DatabaseConnector.h"



/**
This is the backend of the CLI-Application i am building. The backend will use Crow library, Run on a raspberry pi 4.
It will be connected to MySQL Database. Will have multiple features such as login, being able to modify the database using RESTapi.
Other implementations and features as well, such as ToDoList, and more.
**/

int main()
{
	crow::SimpleApp app;
	apiHandler apiHandler;
	DatabaseConnector database("database.db");
	sqlite3* db;
	if (database.connect(db))
	{
		std::cout << "connected to database!" << std::endl;
	}
	else
	{
		std::cout << "Not connected to database" << std::endl;
	}
	
	CROW_ROUTE(app, "/login")
		.methods("GET"_method, "POST"_method)
		([&apiHandler, &database](const crow::request& req) {
		login_handler loginhandler(database);
		return loginhandler.handleRequest(req);
		});

	CROW_ROUTE(app, "/get")([]() {
		return "Hello world";
		});

	CROW_ROUTE(app, "/delete")([]() {
		return "Hello world";
		});

	CROW_ROUTE(app, "/put")([]() {
		return "Hello world";
		});
	
	app.bindaddr("localhost").port(3834).multithreaded().run();

}

