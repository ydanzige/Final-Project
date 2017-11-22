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

bool Client::Login()
{
	value jsonObj;
	jsonObj[L"username"] = value::string(wstring(getUsername().begin(), getUsername().end()));
	jsonObj[L"password"] = value::string(wstring(m_password.begin(), m_password.end()));
	return ExecutePost(jsonObj);
}

void Client::GetActiveUsers()
{
	
}

string Client::GetMessage(string fromUser)
{
	return string();
}

bool Client::Send(string fromUser, string toUser, string message)
{
	value jsonObj;
	jsonObj[ L"fromUser" ] = value::string(wstring(fromUser.begin(), fromUser.end()));
	jsonObj[ L"toUser" ] = value::string(wstring(toUser.begin(), toUser.end()));
	jsonObj[ L"message" ] = value::string(wstring(message.begin(), message.end()));
	return ExecutePost(jsonObj);
	
}

bool Client::SendAll()
{
	return false;
}

bool Client::Ban(string username)
{
	return false;
}

bool Client::Unban(string username)
{
	return false;
}

bool Client::ExecutePost(value jsonObj)
{
	try
	{
		value jsonRes = sendPostRequest(jsonObj).get();
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

pplx::task<value> Client::sendPostRequest(value jsonObj)
{
	const uri serverURI = U("localhost");
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
