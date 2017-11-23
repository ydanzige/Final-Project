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
	class HttpServer
	{
		public:
		HttpServer( utility::string_t url );
		~HttpServer();


		Task<void> open() { return m_listener.open(); }
		Task<void> close() { return m_listener.close(); }


		private:
		http_listener m_listener;
		Vector<unique_ptr<Message>> m_msglst;
		Vector<unique_ptr<User>> m_usrlst;
		unordered_map<utility::string_t, function<http_response( http_request )>> m_routes;

		void HttpPost( http_request message );
		http_response HandleLogin( http_request message );
		http_response HandleGetActiveUser( http_request message );
		http_response HandleSendMessages( http_request message );
		http_response HandleGetMessages( http_request message );
		http_response HandleSendToAllUsers( http_request message );
		http_response HandleBanUser( http_request message );
		http_response HandleUnanUser( http_request message );
		bool AddMsgToLst( string_t fromusername, string_t tousername, string_t massege );
	};
}