#include "stdafx.h"
#include "Server.h"

const string_t DEFAULT_PORT = L"34568";
const Vector<string_t> g_urls
{
	U("login"),
	U("getActiveUser"),
	U("sendMessage"),
	U("getMessage"),
	U("sendToAllUsers"),
	U("banAUser"),
	U("unbanAUser")
};

Vector<unique_ptr<Server::HttpController>> g_srvlst;
unordered_map<utility::string_t, unique_ptr<User>> g_users;
Vector<unique_ptr<Message>> g_messages;

void InitializeServer(const string_t& address)
{
	// Build our listener's URI from the configured address and the hard-coded path "blackjack/dealer"
	uri_builder base(address);

	for (size_t i = 0; i < g_urls.size(); i++)
	{
		uri_builder uri(base);
		uri.append_path(g_urls[i]);

		auto addr = uri.to_uri().to_string();
		g_srvlst.push_back(unique_ptr<Server::HttpController>(new Server::HttpController(addr, g_users, g_messages)));
		g_srvlst[i]->open().wait();
	}

	ucout << string_t(U("Listening for requests at: ")) << base.to_uri().to_string() << std::endl;

	return;
}

void ShutdownServer()
{
	for (size_t i = 0; i < g_urls.size(); i++)
	{
		g_srvlst[i]->close().wait();
	}
	return;
}

inline void WaitForExit()
{
	cout << "Press ENTER to exit..." << std::endl;
	string line;
	getline( std::cin, line );
}

int wmain( int argc, wchar_t * argv[] )
{
	string_t port = argc < 2 ? DEFAULT_PORT : argv[ 1 ];

	string_t address = U("http://localhost:");
	address.append(port);

	InitializeServer(address);
	
	WaitForExit();

	ShutdownServer();
	return 0;
}