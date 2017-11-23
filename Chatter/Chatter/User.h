#pragma once
#include "stdafx.h"

#define String utility::string_t 
#define Vector std::vector 

namespace Server
{
	/**
	Contains user details and bun list of usernames 
	*/
	class User
	{
	public:
		User();
		User(String, String);
		~User();

		bool SetPassword(String);
		String GetUsername() const;
		Vector<String> GetBannedUsers() const;
		void AddBannedUser(String username);
		void RemoveBannedUser(String username);
		bool IsBanned(String username);

	private:
		String m_name;
		String m_pass;
		Vector<String> m_bannedUserList;
	};
}

