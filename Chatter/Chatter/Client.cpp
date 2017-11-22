#include "Client.h"

Client::Client()
{
}


Client::~Client()
{
}

bool Client::Login(string username, string password)
{
	return false;
}

string Client::GetActiveUsers()
{
	return string();
}

string Client::GetMessage(string fromUser)
{
	return string();
}

void Client::Send(string fromUser, string toUser, string message)
{
	value jsonObj;
	jsonObj[ L"fromUser" ] = value::string(wstring(fromUser.begin(), fromUser.end()));
	jsonObj[ L"toUser" ] = value::string(wstring(toUser.begin(), toUser.end()));
	jsonObj[ L"message" ] = value::string(wstring(message.begin(), message.end()));

	sendPostRequest(jsonObj);
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
		return pplx::task_from_result(value());
	});
}
