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
	return true;
}

String Server::User::GetUsername() const
{
	return m_name;
}

Vector<String> Server::User::GetBannedUsers() const
{
	return Vector<String>();
}

void Server::User::AddBannedUser(String username)
{
	this->m_bannedUserList.push_back(username);
}

void Server::User::RemoveBannedUser(String username)
{
	Vector<size_t> indexesToDelete;

	for(size_t i = 0; i < this->m_bannedUserList.size(); i++)
	{
		if(m_bannedUserList[i] == username)
		{
			indexesToDelete.push_back(i);
		}
	}

	for(size_t i = 0; i < indexesToDelete.size(); i++)
	{
		this->m_bannedUserList.erase(m_bannedUserList.begin() + indexesToDelete[i]);
	}
}

bool Server::User::IsBanned(String username)
{
	for(size_t i = 0; i < this->m_bannedUserList.size(); i++)
	{
		if(this->m_bannedUserList[i] == username)
		{
			return true;
		}
	}

	return false;
}
