#include "Client.h"

Client::Client()
{
}

Client::~Client()
{
}

string Client::getUsername()
{
	return m_username;
}

void Client::setUsername(string username)
{
	m_username = username;
}

void Client::setPassword(string password)
{
	m_password = password;
}


const bool Client::Login()
{
	value jsonObj;
	jsonObj[L"username"] = value::string(wstring(getUsername().begin(), getUsername().end()));
	jsonObj[L"password"] = value::string(wstring(m_password.begin(), m_password.end()));
	return ExecutePost(jsonObj, "Login");

}

void Client::GetActiveUsers()
{
	value jsonObj;
	
	try
	{
		value jsonRes = sendPostRequest(jsonObj,"GetActiveUsers").get();
		if (!jsonRes.has_field(U("ErrorRespond")))
		{
			Print(jsonRes);
		}
	}
	catch (const std::exception& e)
	{
		cout << e.what() << '\n';
	}	
}

void Client::GetMessage(const string &fromUser)
{
	value jsonObj;
	jsonObj[L"fromUser"] = value::string(wstring(fromUser.begin(), fromUser.end()));
	while (true)
	{
		try
		{
			value jsonRes = sendPostRequest(jsonObj, "GetMessage").get();
			if (!jsonRes.has_field(U("ErrorRespond")))
			{
				Print(jsonRes);
			}
		}
		catch (const std::exception& e)
		{
			cout << e.what() << '\n';
		}				
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

const bool Client::Send(const string &fromUser, const string &toUser, const string &message)
{
	value jsonObj;
	jsonObj[ L"fromUser" ] = value::string(wstring(fromUser.begin(), fromUser.end()));
	jsonObj[ L"toUser" ] = value::string(wstring(toUser.begin(), toUser.end()));
	jsonObj[ L"message" ] = value::string(wstring(message.begin(), message.end()));
	return ExecutePost(jsonObj, "Send");
}

const bool Client::SendToAllUsers(const string &fromUser, const string &message)
{
	value jsonObj;
	jsonObj[L"fromUser"] = value::string(wstring(fromUser.begin(), fromUser.end()));
	jsonObj[L"message"] = value::string(wstring(message.begin(), message.end()));
	return ExecutePost(jsonObj, "SendToAllUsers");
}

const bool Client::Ban(const string &fromUser, const string &toUser)
{
	value jsonObj;
	jsonObj[L"fromUser"] = value::string(wstring(fromUser.begin(), fromUser.end()));
	jsonObj[L"toUser"] = value::string(wstring(toUser.begin(), toUser.end()));
	return ExecutePost(jsonObj, "banAUser");
}

const bool Client::Unban(const string &fromUser, const string &toUser)
{
	value jsonObj;
	jsonObj[L"fromUser"] = value::string(wstring(fromUser.begin(), fromUser.end()));
	jsonObj[L"toUser"] = value::string(wstring(toUser.begin(), toUser.end()));
	return ExecutePost(jsonObj, "unbanAUser");
}

const bool Client::ExecutePost(const value &jsonObj,const string &funcName)
{
	try
	{
		value jsonRes = sendPostRequest(jsonObj, funcName).get();
		if (jsonRes.has_field(U("ErrorRespond")))
		{
			return false;
		}
	}
	catch (const std::exception&)
	{
		return false;
	}
	return true;
}

void Client::Print(const value &json)const
{
	wcout << L"From User: " << json.at(U("fromUser")).as_string() << L"->"<< json.at(U("message")).as_string();
}

pplx::task<value> Client::sendPostRequest(const value &jsonObj,const string &funcName)
{
	const uri serverURI = U("localhost/") + wstring(funcName.begin(),funcName.end());
	client::http_client client(serverURI);

	// Manually build up an HTTP request with header and request URI.
	http_request request(methods::POST);
	request.headers().add(L"Content-Type", L"application/json");
	request.headers().add(L"Content-Length", L"100");
	request.headers().add(L"Host", L"example.com");
	request.headers().add(L"X-Requested-With", L"XMLHttpRequest");
	request.set_body(jsonObj);
	return client.request(request).then([](http_response response)
	{
		if (response.status_code() == status_codes::OK)
		{
			return response.extract_json();
		}
		else {
			/* Print bad status code */
			wcout << L"Server returned returned status code " << response.status_code() << L'.' << std::endl;
		}
		value jsonObj;
		jsonObj[L"ErrorRespond"] = value::string(U("Error"));
		return pplx::task_from_result(jsonObj);
	});
}

