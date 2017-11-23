#pragma once
#include "stdafx.h"

#define String utility::string_t 

namespace Server
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
