#include <string>
#include <vector>

namespace Server
{
	class User
	{
	public:
		User();
		~User();

	private:
		std::string m_name;
		std::string m_pass;
		std::vector<std::string> m_bunUserList;
	};

	

	class DirectMessage : public Message
	{
	public:
		DirectMessage();
		~DirectMessage();
		bool SendToUser(std::string userTo);
	private:
		std::string m_to;
	};

	
	Message::Message()
	{
	}

	Message::~Message()
	{
	}
}

