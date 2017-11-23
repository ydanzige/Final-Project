#include "stdafx.h"

#include "Client.h"

using namespace web::http;

Client::Client()
{
}


Client::~Client()
{
}

bool Client::Login(string username, string password)
{
	return false;
}

string Client::GetActiveUsers()
{
	return string();
}

string Client::GetMessage(string fromUser)
{
	return string();
}

bool Client::Send(string fromUser, string toUser, string message)
{
	return false;
}

bool Client::SendAll()
{
	return false;
}

bool Client::Ban(string username)
{
	return false;
}

bool Client::Unban(string username)
{
	return false;
}
