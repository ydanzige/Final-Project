#pragma once

#include <string>
using namespace std;

class Client
{
private:
	string m_username;
	string m_serverName;
	int m_port;
public:
	Client();
	~Client();

	bool Login(string serverName);
};

