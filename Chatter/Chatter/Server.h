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

REQUEST_ID Server::getId(web::uri uri)
{
	if (uri.path().c_str() == L"login")
	{
		return REQUEST_ID::LOGIN;
	}
	else if (uri.path().c_str() == L"getActiveUser")
	{
		return REQUEST_ID::GET_ACTIVE_USERS;
	}
	else if (uri.path().c_str() == L"sendMessage")
	{
		return REQUEST_ID::SEND_MESSAGES;
	}
	else if (uri.path().c_str() == L"getMessage")
	{
		return REQUEST_ID::GET_MESSAGES;
	}
	else if (uri.path().c_str() == L"sendToAllUsers")
	{
		return REQUEST_ID::SENT_TO_ALL_USERS;
	}
	else if (uri.path().c_str() == L"banAUser")
	{
		return REQUEST_ID::BAN_USER;
	}
	else if (uri.path().c_str() == L"unbanAUser")
	{
		return REQUEST_ID::UNBAN_USER;
	}
	else
	{
		return REQUEST_ID::NONE;
	}
}

http_response Server::HandleLogin(http_request message)
{
	auto body = message.extract_json().get();
	web::json::value username = body[L"username"];
	web::json::value password = body[L"password"];
	m_usrlst.push_back(std::unique_ptr<ServerNS::User>(new ServerNS::User(username.as_string(), password.as_string())));
	http_response res(200);
	return res;
}
http_response Server::HandleGetActiveUser(http_request message)
{
	web::json::value obj;
	web::json::value arr;
	std::vector<web::json::value> users;
	for (int i = 0; i < m_usrlst.size(); i++)
	{
		users.push_back(web::json::value(m_usrlst[i]->GetUsername()));
	}
	arr.array(users);
	http_response res(200);
	res.set_body(arr);
	return res;
}
http_response Server::HandleSendMessages(http_request message)
{
	auto body = message.extract_json().get();
	web::json::value fromusername = body[L"fromusername"];
	web::json::value tousername = body[L"tousername"];
	web::json::value massege = body[L"massege"];

	http_response res(500);
	for (size_t i = 0; i < m_usrlst.size(); i++)
	{
		if (m_usrlst[i]->GetUsername() == tousername.as_string())
		{
			for (size_t j = 0; i < m_usrlst[i]->GetBunUsers().size(); i++)
			{
				if (m_usrlst[i]->GetBunUsers()[j] == fromusername.as_string())
				{
					return res;
				}
			}
			m_msglst.push_back(std::unique_ptr<Message>(new DirectMessage(fromusername.to_string(), tousername.to_string(), massege.to_string())));
			res.set_status_code(200);
			return res;
		}
	}
	return res;
}
http_response Server::HandleGetMessages(http_request message)
{
	return true;
}
http_response Server::HandleSendToAllUsers(http_request message)
{
	return true;
}
http_response Server::HandleBanUser(http_request message)
{
	return true;
}
http_response Server::HandleUnanUser(http_request message)
{
	return true;
}
