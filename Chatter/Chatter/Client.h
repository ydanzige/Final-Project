#pragma once

#include "cpprest\http_client.h"
#include <string>

using namespace std;
using namespace web::http;
using namespace web::json;

class Client
{
private:
	const string SERVER = "server";
	const int PORT = 1234;

	string m_username;
	string m_password;
public:
	Client();
	~Client();

	string getUsername();
	void setUsername(string username);
	void setPassword(string username);
	const bool Login();
	void GetActiveUsers();
	static void GetMessage(const string &fromUser);
	const bool Send(const string &fromUser, const string &toUser, const string &message);
	const bool SendToAllUsers(const string &fromUser, const string &message);
	const bool Ban(const string &fromUser, const string &toUser);
	const bool Unban(const string &fromUser, const string &toUser);
	const bool ExecutePost(const value &json, const string&funcName);
	void Print(const value &json) const;
	pplx::task<value> sendPostRequest(const value &jsonObj, const string &funcName);
};

