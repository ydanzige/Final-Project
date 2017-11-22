#include <string>
#include <ctime>
#include <time.h>
#include <stdio.h>

#define String std::string

namespace ServerNS
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
