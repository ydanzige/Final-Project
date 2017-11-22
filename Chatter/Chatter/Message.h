#include <string>

namespace Server
{
	class Message
	{
	public:
		Message();
		~Message();
	private:
		std::string m_message;
		std::string m_from;
	};
}
