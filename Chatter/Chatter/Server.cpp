#pragma once
#include "Server.h"

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
	REQUEST_ID id = getId(message.absolute_uri());
	http_response res(500);
	switch (id)
	{
	case LOGIN:
		res = HandleLogin(message);
		break;
	case GET_ACTIVE_USERS:
		res = HandleGetActiveUser(message);
		break;
	case SEND_MESSAGES:
		res = HandleSendMessages(message);
		break;
	case GET_MESSAGES:
		res = HandleGetMessages(message);
		break;
	case SENT_TO_ALL_USERS:
		res = HandleSendToAllUsers(message);
		break;
	case BAN_USER:
		res = HandleBanUser(message);
		break;
	case UNBAN_USER:
		res = HandleUnanUser(message);
		break;
	default:
		break;
	}
	message.reply(res);
}

REQUEST_ID Server::getId(web::uri uri)
{
	if (0 == wcscmp(uri.path().c_str(), L"/login"))
	{
		return REQUEST_ID::LOGIN;
	}
	else if (0 == wcscmp(uri.path().c_str(), L"/getActiveUser"))
	{
		return REQUEST_ID::GET_ACTIVE_USERS;
	}
	else if (0 == wcscmp(uri.path().c_str(), L"/sendMessage"))
	{
		return REQUEST_ID::SEND_MESSAGES;
	}
	else if (0 == wcscmp(uri.path().c_str(), L"/getMessage"))
	{
		return REQUEST_ID::GET_MESSAGES;
	}
	else if (0 == wcscmp(uri.path().c_str(), L"/sendToAllUsers"))
	{
		return REQUEST_ID::SENT_TO_ALL_USERS;
	}
	else if (0 == wcscmp(uri.path().c_str(), L"/banAUser"))
	{
		return REQUEST_ID::BAN_USER;
	}
	else if (0 == wcscmp(uri.path().c_str(), L"/unbanAUser"))
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
	web::json::value username;
	web::json::value password;
	message.extract_json().then([&](pplx::task<json::value> previousTask) {
		auto body = previousTask.get();
		username = body[L"username"];
		password = body[L"password"];
	}).wait();
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
	web::json::value fromusername;
	web::json::value tousername;
	web::json::value massege;
	message.extract_json().then([&](pplx::task<json::value> previousTask) {
		auto body = previousTask.get();
		fromusername = body[L"fromusername"];
		tousername = body[L"tousername"];
		massege = body[L"massege"];
	}).wait();
	http_response res;
	bool success = AddMsgToLst(fromusername.as_string(), tousername.as_string(), massege.as_string());
	if (success)
	{
		res.set_status_code(200);
	}
	else
	{
		res.set_status_code(500);
	}
	return res;

}

http_response Server::HandleGetMessages(http_request message)
{
	web::json::value username;
	message.extract_json().then([&](pplx::task<json::value> previousTask) {
		auto body = previousTask.get();
		username = body[L"username"];
	}).wait();
	
	web::json::value arr;
	std::vector<web::json::value> masseges;

	for (size_t i = 0; i < m_msglst.size(); i++)
	{
		if (m_msglst[i]->GetTo().compare(username.as_string()))
		{
			masseges.push_back(web::json::value(m_msglst[i]->GetContent()));
		}
	}
	arr.array(masseges);
	http_response res(200);
	res.set_body(arr);
	return res;
}

http_response Server::HandleSendToAllUsers(http_request message)
{
	web::json::value fromusername;
	web::json::value massege;
	message.extract_json().then([&](pplx::task<json::value> previousTask) {
		auto body = previousTask.get();
		fromusername = body[L"fromusername"];
		massege = body[L"massege"];
	}).wait();
	for (size_t i = 0; i < m_usrlst.size(); i++)
	{
		AddMsgToLst(fromusername.as_string(), m_usrlst[i]->GetUsername(), massege.as_string());
	}
	return http_response(200);
}

http_response Server::HandleBanUser(http_request message)
{
	web::json::value fromusername;
	web::json::value banusername;
	message.extract_json().then([&](pplx::task<json::value> previousTask) {
		auto body = previousTask.get();
		fromusername = body[L"fromusername"];
		banusername = body[L"banusername"];
	}).wait();

	for (size_t i = 0; i < m_usrlst.size(); i++)
	{
		if (m_usrlst[i]->GetUsername().compare(fromusername.as_string()))
		{
			m_usrlst[i]->GetBunUsers().push_back(banusername.as_string());
			break;
		}
	}
	return http_response(200);
}

http_response Server::HandleUnanUser(http_request message)
{
	web::json::value fromusername;
	web::json::value unbanusername;
	message.extract_json().then([&](pplx::task<json::value> previousTask) {
		auto body = previousTask.get();
		fromusername = body[L"fromusername"];
		unbanusername = body[L"unbanusername"];
	}).wait();


	for (size_t i = 0; i < m_usrlst.size(); i++)
	{
		if (m_usrlst[i]->GetUsername().compare(fromusername.as_string()))
		{
			for (size_t j = 0; i < m_usrlst[i]->GetBunUsers().size(); i++)
			{
				if (m_usrlst[i]->GetBunUsers()[j].compare(unbanusername.as_string()))
				{
					m_usrlst[i]->GetBunUsers().erase(m_usrlst[i]->GetBunUsers().begin() + j);
					break;
				}
			}
			break;
		}
	}
	return http_response(200);
}

bool Server::AddMsgToLst(string_t fromusername, string_t tousername, string_t massege)
{
	for (size_t i = 0; i < m_usrlst.size(); i++)
	{
		if (m_usrlst[i]->GetUsername().compare(tousername))
		{
			for (size_t j = 0; i < m_usrlst[i]->GetBunUsers().size(); i++)
			{
				if (m_usrlst[i]->GetBunUsers()[j].compare(fromusername))
				{
					return false;
				}
			}
			m_msglst.push_back(std::unique_ptr<Message>(new Message(fromusername,
				tousername,
				massege)));
			return true;
		}
	}
	return false;
}
