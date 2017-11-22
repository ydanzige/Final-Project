#pragma once
#include "Message.h"

namespace ServerNS 
{
	class DirectMessage : public Message
	{
	public:
		DirectMessage();
		~DirectMessage();
		bool SendToUser(String);
	private:
		String m_to;
	};
}

