#include "Server.h"

std::unique_ptr<Server> g_httpDealer;

void on_initialize(const string_t& address)
{
	// Build our listener's URI from the configured address and the hard-coded path "blackjack/dealer"

	uri_builder uri(address);
	uri.append_path(U("blackjack/dealer"));

	auto addr = uri.to_uri().to_string();
	g_httpDealer = std::unique_ptr<Server>(new Server(addr));
	g_httpDealer->open().wait();

	ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;

	return;
}

void on_shutdown()
{
	g_httpDealer->close().wait();
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