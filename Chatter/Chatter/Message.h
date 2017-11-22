#include <string>
#include <ctime>

#define String std::string

namespace Server
{
	/**
	Contains message details
	*/
	class Message
	{
	public:
		Message();
		~Message();

		String GetContent() const;
		String GetTimestamp() const;
	private:
		String m_message;
		String m_from;
		time_t m_timestamp;
	};
}
