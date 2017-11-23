#pragma once
#include "Server.h"
#include "Helpers.h"

Server::HttpServer::HttpServer(utility::string_t url) : m_listener(url)
{
	m_listener.support(methods::POST, bind(&Server::HttpServer::HttpPost, this, placeholders::_1));

	m_routes[ L"/login" ] = std::bind( &Server::HttpServer::HandleLogin, this, placeholders::_1 );
	m_routes[ L"/getActiveUser" ] = std::bind( &Server::HttpServer::HandleGetActiveUser, this, placeholders::_1 );
	m_routes[ L"/sendMessage" ] = std::bind( &Server::HttpServer::HandleSendMessages, this, placeholders::_1 );
	m_routes[ L"/getMessage" ] = std::bind( &Server::HttpServer::HandleGetMessages, this, placeholders::_1 );
	m_routes[ L"/sendToAllUsers" ] = std::bind( &Server::HttpServer::HandleSendToAllUsers, this, placeholders::_1 );
	m_routes[ L"/banAUser" ] = std::bind( &Server::HttpServer::HandleBanUser, this, placeholders::_1 );
	m_routes[ L"/unbanAUser" ] = std::bind( &Server::HttpServer::HandleUnanUser, this, placeholders::_1 );
}

Server::HttpServer::~HttpServer()
{}

void Server::HttpServer::HttpPost( http_request request )
{
	auto path = request.absolute_uri().path();
	m_routes[ path ]( request );	
}

http_response Server::HttpServer::HandleLogin(http_request request)
{
	web::json::value username;
	web::json::value password;
	request.extract_json().then([&](pplx::task<json::value> previousTask) {
		auto body = previousTask.get();
		username = body[L"username"];
		password = body[L"password"];
	}).wait();

	try
	{
		m_usrlst.push_back( std::unique_ptr<Server::User>( new Server::User( username.as_string(), password.as_string() ) ) );
	}
	catch( ... )
	{
		http_response res( 500 );
		return res;
	}
	
	http_response res(200);
	return res;
}

http_response Server::HttpServer::HandleGetActiveUser(http_request request)
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

http_response Server::HttpServer::HandleSendMessages(http_request request)
{
	web::json::value fromusername;
	web::json::value tousername;
	web::json::value massege;
	request.extract_json().then([&](pplx::task<json::value> previousTask) {
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

http_response Server::HttpServer::HandleGetMessages(http_request request)
{
	web::json::value username;
	request.extract_json().then([&](pplx::task<json::value> previousTask) {
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

http_response Server::HttpServer::HandleSendToAllUsers(http_request request)
{
	web::json::value fromusername;
	web::json::value massege;
	request.extract_json().then([&](pplx::task<json::value> previousTask) {
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

http_response Server::HttpServer::HandleBanUser(http_request request)
{
	web::json::value fromusername;
	web::json::value banusername;
	request.extract_json().then([&](pplx::task<json::value> previousTask) {
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

http_response Server::HttpServer::HandleUnanUser(http_request request)
{
	web::json::value fromusername;
	web::json::value unbanusername;
	request.extract_json().then([&](pplx::task<json::value> previousTask) {
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

bool Server::HttpServer::AddMsgToLst(string_t fromusername, string_t tousername, string_t message)
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
			m_msglst.push_back( std::unique_ptr<Message>( new Message( fromusername,
				tousername, message ) ) );
			return true;
		}
	}
	return false;
}
