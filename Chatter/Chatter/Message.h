#include <string>
#include <ctime>

#define String std::string

namespace ServerNS
{
	class Message
	{
	public:
		Message();
		~Message();
	private:
		String m_message;
		String m_from;
		time_t m_timestamp;
	};
}
