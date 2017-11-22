#include <cpprest\http_listener.h>
#include <cpprest\json.h>
#include <cpprest\uri.h>
#include <cpprest\asyncrt_utils.h>
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
	bool HandleLogin(http_request message);
	bool HandleGetActiveUser(http_request message);
	bool HandleSendMessages(http_request message);
	bool HandleGetMessages(http_request message);
	bool HandleSendToAllUsers(http_request message);
	bool HandleBanUser(http_request message);
	bool HandleUnanUser(http_request message);
	REQUEST_ID getId(web::uri);
};

Server::Server()
{
}

Server::Server(utility::string_t url) : m_listener(url)
{
	m_listener.support(methods::POST, std::bind(&Server::handle_post, this, std::placeholders::_1));
}

Server::~Server()
{
}

void Server::handle_post(http_request message)
{
	REQUEST_ID id = getId(message.relative_uri());
	switch (id)
	{
	case LOGIN:
		HandleLogin(message);
		break;
	case GET_ACTIVE_USERS:
		HandleGetActiveUser(message);
		break;
	case SEND_MESSAGES:
		HandleSendMessages(message);
		break;
	case GET_MESSAGES:
		HandleGetMessages(message);
		break;
	case SENT_TO_ALL_USERS:
		HandleSendToAllUsers(message);
		break;
	case BAN_USER:
		HandleBanUser(message);
		break;
	case UNBAN_USER:
		HandleUnanUser(message);
		break;
	default:
		break;
	}
}

REQUEST_ID Server::getId(web::uri)
{
	return LOGIN;
}

bool Server::HandleLogin(http_request message)
{
	auto body = message.extract_json().get();
	web::json::value username = body[L"username"];
	web::json::value password = body[L"password"];
	m_usrlst.push_back(std::unique_ptr<User>(new User(username.as_string(), password.as_string())));
	return true;
}

bool Server::HandleGetActiveUser(http_request message)
{
	web::json::value obj;
	web::json::value arr;
	std::vector<web::json::value> users;
	for (int i = 0; i < m_usrlst.size(); i++)
	{
		users.push_back(web::json::value(m_usrlst[i]->GetUsername()));
	}
	arr.array(users);
	return true;
}

bool Server::HandleSendMessages(http_request message)
{
	return true;
}
bool Server::HandleGetMessages(http_request message)
{
	return true;
}
bool Server::HandleSendToAllUsers(http_request message)
{
	return true;
}
bool Server::HandleBanUser(http_request message)
{
	return true;
}
bool Server::HandleUnanUser(http_request message)
{
	return true;
}
