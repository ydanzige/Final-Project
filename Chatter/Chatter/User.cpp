#include "User.h"

Server::User::User()
{}

Server::User::User(String username, String password)
	: m_name(username), m_pass(password)
{}

Server::User::~User()
{}

bool Server::User::SetPassword(String newPassword)
{
	// Todo: check for password restrictions
	this->m_pass.assign(newPassword);
}

String Server::User::GetUsername() const
{
	return m_name;
}

Vector<String> Server::User::GetBunUsers() const
{
	return Vector<String>();
}
