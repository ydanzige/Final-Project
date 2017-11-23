#pragma once
#include "stdafx.h"

#include "DirectMessage.h"
#include "Message.h"
#include "User.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;
using namespace Server;
using namespace std;

#define Vector std::vector
#define Task pplx::task

namespace Server
{
	class HttpController
	{
		public:
		HttpController( utility::string_t url, unordered_map<utility::string_t, unique_ptr<User>> &users, Vector<unique_ptr<Message>> &messages );
		~HttpController();


		Task<void> open() { return m_listener.open(); }
		Task<void> close() { return m_listener.close(); }


		private:
		http_listener m_listener;
		Vector<unique_ptr<Message>> &m_msglst;
		unordered_map<utility::string_t, unique_ptr<User>> &m_usersList;
		unordered_map<utility::string_t, function<void( http_request )>> m_routes;

		void HttpPost( http_request message );
		void HandleLogin( http_request message );
		void HandleGetActiveUser( http_request message );
		void HandleSendMessages( http_request message );
		void HandleGetMessages( http_request message );
		void HandleSendToAllUsers( http_request message );
		void HandleBanUser( http_request message );
		void HandleUnBanUser( http_request message );
		bool AddMsgToLst( string_t fromusername, string_t tousername, string_t massege );
	};
}