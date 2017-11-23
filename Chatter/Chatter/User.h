#pragma once
#include "stdafx.h"

using namespace utility;
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
		Vector<String> GetBunUsers() const;

	private:
		String m_name;
		String m_pass;
		Vector<String> m_bunUserList;
	};
}

