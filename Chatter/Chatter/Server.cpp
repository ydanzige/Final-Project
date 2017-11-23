#pragma once
#include "Server.h"
#include "Helpers.h"

Server::HttpController::HttpController(utility::string_t url, unordered_map<utility::string_t, unique_ptr<User>> &users, Vector<unique_ptr<Message>> &messages)
	: m_listener(url), m_usersList(users), m_msglst(messages)
{
	m_listener.support(methods::POST, bind(&Server::HttpController::HttpPost, this, placeholders::_1));

	m_routes[L"/login"] = std::bind(&Server::HttpController::HandleLogin, this, placeholders::_1);
	m_routes[L"/getActiveUser"] = std::bind(&Server::HttpController::HandleGetActiveUser, this, placeholders::_1);
	m_routes[L"/sendMessage"] = std::bind(&Server::HttpController::HandleSendMessages, this, placeholders::_1);
	m_routes[L"/getMessage"] = std::bind(&Server::HttpController::HandleGetMessages, this, placeholders::_1);
	m_routes[L"/sendToAllUsers"] = std::bind(&Server::HttpController::HandleSendToAllUsers, this, placeholders::_1);
	m_routes[L"/banAUser"] = std::bind(&Server::HttpController::HandleBanUser, this, placeholders::_1);
	m_routes[L"/unbanAUser"] = std::bind(&Server::HttpController::HandleUnBanUser, this, placeholders::_1);

}

Server::HttpController::~HttpController()
{
}

void Server::HttpController::HttpPost(http_request request)
{
	auto path = request.absolute_uri().path();
	m_routes[path](request);
}

void Server::HttpController::HandleLogin(http_request request)
{
	web::json::value retVal;
	try
	{
		web::json::value username;
		web::json::value password;


		request.extract_json().then([&](pplx::task<json::value> previousTask) {
			auto body = previousTask.get();
			username = body[L"username"];
			password = body[L"password"];
		}).wait();
		m_usersList[username.as_string()] = std::unique_ptr<Server::User>(new Server::User(username.as_string(), password.as_string()));

		retVal[L"message"] = web::json::value(L"User Logged In");
		request.reply(status_codes::OK, retVal);
	}
	catch(...)
	{

		retVal[L"type"] = web::json::value(L"Error");
		retVal[L"message"] = web::json::value(L"Could not login user");
		request.reply(status_codes::NotAcceptable, retVal);
	}
}

void Server::HttpController::HandleGetActiveUser(http_request request)
{
	try
	{
		web::json::value arr;
		std::vector<web::json::value> users;
		for(auto it = m_usersList.begin(); it != m_usersList.end(); it++)
		{
			arr[it->first] = web::json::value(it->second->GetUsername());
		}

		request.reply(status_codes::OK, arr);
	}
	catch(...)
	{
		request.reply(status_codes::BadRequest);
	}
}

void Server::HttpController::HandleSendMessages(http_request request)
{
	try
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

		bool success = AddMsgToLst(fromusername.as_string(), tousername.as_string(), massege.as_string());
		if(success)
		{
			request.reply(status_codes::OK, web::json::value(L"Message was sent"));
		}
		else
		{
			request.reply(status_codes::InternalError, web::json::value(L"Message was not sent"));
		}
	}
	catch(...)
	{
		request.reply(status_codes::InternalError, web::json::value(L"Message was not sent"));
	}

}

void Server::HttpController::HandleGetMessages(http_request request)
{
	try
	{
		web::json::value username;
		request.extract_json().then([&](pplx::task<json::value> previousTask) {
			auto body = previousTask.get();
			username = body[L"username"];
		}).wait();

		web::json::value arr;

		auto &user = m_usersList[username.as_string()];
		if(nullptr != user)
		{
			for(size_t i = 0, j = 0; i < m_msglst.size(); i++)
			{
				if(m_msglst[i]->GetTo() == username.as_string() && !user->IsBanned(m_msglst[i]->GetFrom()))
				{
					web::json::value message;
					message[L"from"] = web::json::value(m_msglst[i]->GetFrom());
					message[L"content"] = web::json::value(m_msglst[i]->GetContent());
					arr[j++] = message;
				}
			}

			request.reply(status_codes::OK, arr);
		}
		else
		{
			request.reply(status_codes::InternalError, web::json::value(L"No such user"));
		}

		
	}
	catch(...)
	{
		request.reply(status_codes::InternalError);
	}
}

void Server::HttpController::HandleSendToAllUsers(http_request request)
{
	try
	{
		web::json::value fromusername;
		web::json::value message;
		request.extract_json().then([&](pplx::task<json::value> previousTask) {
			auto body = previousTask.get();
			fromusername = body[L"fromusername"];
			message = body[L"massege"];
		}).wait();

		for(auto it = m_usersList.begin(); it != m_usersList.end(); it++)
		{
			AddMsgToLst(fromusername.as_string(), it->first, message.as_string());
		}

		request.reply(status_codes::OK, web::json::value(L"Message was sent"));
	}
	catch(...)
	{
		request.reply(status_codes::BadRequest, web::json::value(L"Message was not sent"));
	}
}

void Server::HttpController::HandleBanUser(http_request request)
{
	try
	{
		web::json::value fromusername;
		web::json::value banusername;
		request.extract_json().then([&](pplx::task<json::value> previousTask) {
			auto body = previousTask.get();
			fromusername = body[L"fromusername"];
			banusername = body[L"banusername"];
		}).wait();


		auto &user = m_usersList[fromusername.as_string()];
		if(nullptr != user)
		{
			user->AddBannedUser(banusername.as_string());
		}
		
		request.reply(status_codes::OK, web::json::value(L"User was banned"));
	}
	catch(...)
	{
		request.reply(status_codes::InternalError, web::json::value(L"User was not banned"));
	}
}

void Server::HttpController::HandleUnBanUser(http_request request)
{
	try
	{
		web::json::value fromusername;
		web::json::value unbanusername;
		request.extract_json().then([&](pplx::task<json::value> previousTask) {
			auto body = previousTask.get();
			fromusername = body[L"fromusername"];
			unbanusername = body[L"unbanusername"];
		}).wait();


		auto &user = m_usersList[fromusername.as_string()];
		if(nullptr != user)
		{
			user->RemoveBannedUser(unbanusername.as_string());
		}

		request.reply(status_codes::OK, web::json::value(L"User was unbanned"));
	}
	catch(...)
	{
		request.reply(status_codes::OK, web::json::value(L"User was not unbanned"));
	}
}

bool Server::HttpController::AddMsgToLst(string_t fromUsername, string_t toUsername, string_t message)
{
	auto &user = m_usersList[fromUsername];
	if(nullptr != user)
	{
		const auto &bunList = user->GetBannedUsers();
		for(size_t i = 0; i < bunList.size(); i++)
		{
			if(bunList[i] == fromUsername)
			{
				return false;
			}
		}

		m_msglst.push_back(std::unique_ptr<Message>(new Message(fromUsername, toUsername, message)));

		return true;
	}

	return false;
}
