#include "User.h"

ServerNS::User::User()
{}

ServerNS::User::User(String username, String password)
	: m_name(username), m_pass(password)
{}

ServerNS::User::~User()
{}

bool ServerNS::User::SetPassword(String newPassword)
{
	// Todo: check for password restrictions
	this->m_pass.assign(newPassword);
}

String ServerNS::User::GetUsername() const
{
	return m_name;
}

Vector<String> ServerNS::User::GetBunUsers() const
{
	return Vector<String>();
}
