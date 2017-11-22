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

<<<<<<< HEAD
	string getUsername();
	void setUsername(string username);
	void setPassword(string username);
	bool Login();
	void GetActiveUsers();	
	string GetMessage(string fromUser);	
	bool Send(string fromUser, string toUser, string message);
	bool SendAll();	
	bool Ban(string username);		
	bool Unban(string username);	
	bool ExecutePost(value json);
	pplx::task<value> sendPostRequest(value jsonObj);
=======
	const bool Login(const string &username, const string &password) ;
	void GetActiveUsers() ;
	void GetMessage(const string &fromUser) ;
	const bool Send(const string &fromUser, const string &toUser, const string &message) ;
	const bool SendToAllUsers(const string &fromUser, const string &message) ;
	const bool Ban(const string &fromUser, const string &toUser) ;
	const bool Unban(const string &fromUser, const string &toUser) ;
	const bool ExecutePost(const value &json ,const string&funcName) ;
	void Print(const value &json) const;
	pplx::task<value> sendPostRequest(const value &jsonObj ,const string &funcName);
>>>>>>> a9a0ceb0c228f5e67c7972dab5574c2c50e889de
};

