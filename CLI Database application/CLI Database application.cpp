#include <iostream>
#include <crow.h>
#include <mysqlx/xdevapi.h>
#include "login_handler.h"
#include "api_handler.h"



/**
This is the backend of the CLI-Application i am building. The backend will use Crow library, Run on a raspberry pi 4.
It will be connected to MySQL Database. Will have multiple features such as login, being able to modify the database using RESTapi.
Other implementations and features as well, such as ToDoList, and more.
**/

int main()
{
	crow::SimpleApp app;
	apiHandler apiHandler;

	CROW_ROUTE(app, "/login")
		.methods("GET"_method)
		([&apiHandler](const crow::request& req) {
		login_handler loginhandler;
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
	app.port(18080).multithreaded().run();
}

