#include <cpprest\http_listener.h>
#include <cpprest\json.h>
#include <cpprest\uri.h>
#include <cpprest\asyncrt_utils.h>

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

class Server
{
public:
	Server()
	{}
	Server(utility::string_t url);
	~Server();


	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }


private:
	void handle_post(http_request message);
	http_listener m_listener;
};

Server::Server()
{
}

Server::Server(utility::string_t url) : m_listener(url)
{
	m_listener.support(methods::POST, std::bind(&Server::handle_post, this, std::placeholders::_1));

	utility::ostringstream_t nextIdString;
	nextIdString << nextId;

	std::shared_ptr<DealerTable> tbl = std::make_shared<DealerTable>(nextId, 8, 6);
	s_tables[nextIdString.str()] = tbl;
	nextId += 1;
}

Server::~Server()
{
}

