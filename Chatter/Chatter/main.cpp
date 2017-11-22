#include "Server.h"

std::vector<std::unique_ptr<Server>> g_srvlst;
std::vector<utility::string_t> g_urls{
	U("login"),
	U("getActiveUser"),
	U("sendMessage"),
	U("getMessage"),
	U("sendToAllUsers"),
	U("banAUser"),
	U("unbanAUser")
};

void on_initialize(const string_t& address)
{
	// Build our listener's URI from the configured address and the hard-coded path "blackjack/dealer"
	uri_builder base(address);

	for (size_t i = 0; i < g_urls.size(); i++)
	{
		uri_builder uri(base);
		uri.append_path(g_urls[i]);

		auto addr = uri.to_uri().to_string();
		g_srvlst.push_back(std::unique_ptr<Server>(new Server(addr)));
		g_srvlst[i]->open().wait();
	}

	ucout << utility::string_t(U("Listening for requests at: ")) << base.to_uri().to_string() << std::endl;

	return;
}

void on_shutdown()
{
	for (size_t i = 0; i < g_urls.size(); i++)
	{
		g_srvlst[i]->close().wait();
	}
	return;
}


int main(void)
{
	utility::string_t port = U("34568");

	utility::string_t address = U("http://localhost:");
	address.append(port);

	on_initialize(address);
	std::cout << "Press ENTER to exit." << std::endl;

	std::string line;
	std::getline(std::cin, line);

	on_shutdown();
	return 0;
}