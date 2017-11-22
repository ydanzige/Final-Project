#pragma once
#include <cpprest\asyncrt_utils.h>
#include <string>
#include <ctime>
#include <time.h>
#include <stdio.h>

using namespace utility;
#define String utility::string_t 

namespace ServerNS
{
	/**
	Contains message details
	*/
	class Message
	{
	public:
		Message(String from, String to, String content);
		~Message();

		String GetContent() const;
		String GetTimestamp() const;
		String GetFrom() const;
		String GetTo() const;
	private:
		String m_message;
		String m_from;
		String m_to;
		time_t m_timestamp;
	};
}
