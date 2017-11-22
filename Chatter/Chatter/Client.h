#pragma once

#include <string>
using namespace std;

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

	bool Login(string username, string password);
	string GetActiveUsers();
	string GetMessage(string fromUser);
	bool Send(string fromUser, string toUser, string message);
	bool SendAll();
	bool Ban(string username);
	bool Unban(string username);

};

