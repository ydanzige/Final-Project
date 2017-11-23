#pragma once
#include "Message.h"

namespace Server 
{
	class DirectMessage : public Message
	{
	public:
		DirectMessage(String from, String to, String content);
		~DirectMessage();
		bool SendToUser(String);
	private:
		String m_to;
	};
}

