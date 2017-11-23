#pragma once
#include "stdafx.h"

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

	bool Login(string username, string password);	// login?username=<username>&password=<password>
	string GetActiveUsers();	// getActiveUser
	string GetMessage(string fromUser);	// getMessages?username=<username>
	bool Send(string fromUser, string toUser, string message);	// send?username=<username>&massege=<massege>
	bool SendAll();	// sendAll?username=<username>&massege=<massege>
	bool Ban(string username);		// banUser
	bool Unban(string username);	// unbanUser
};

