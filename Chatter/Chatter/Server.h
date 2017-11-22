#pragma once
#include <cpprest\http_listener.h>
#include <cpprest\json.h>
#include <cpprest\uri.h>
#include <cpprest\asyncrt_utils.h>
#include "DirectMessage.h"
#include "Message.h"
#include "User.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;
using namespace ServerNS;

enum REQUEST_ID
{
	LOGIN,
	GET_ACTIVE_USERS,
	SEND_MESSAGES,
	GET_MESSAGES,
	SENT_TO_ALL_USERS,
	BAN_USER,
	UNBAN_USER,
	NONE
};

class Server
{
public:
	Server();
	Server(utility::string_t url);
	~Server();


	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }


private:
	http_listener m_listener;
	std::vector<std::unique_ptr<ServerNS::Message>> m_msglst;
	std::vector<std::unique_ptr<ServerNS::User>> m_usrlst;
	//
	void handle_post(http_request message);
	http_response HandleLogin(http_request message);
	http_response HandleGetActiveUser(http_request message);
	http_response HandleSendMessages(http_request message);
	http_response HandleGetMessages(http_request message);
	http_response HandleSendToAllUsers(http_request message);
	http_response HandleBanUser(http_request message);
	http_response HandleUnanUser(http_request message);
	bool AddMsgToLst(string_t fromusername, string_t tousername, string_t massege);
	REQUEST_ID getId(web::uri);
};