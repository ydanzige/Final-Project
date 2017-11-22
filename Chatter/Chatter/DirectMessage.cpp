#include "DirectMessage.h"

ServerNS::DirectMessage::DirectMessage(String from, String to, String content) : Message(from, content), m_to(to)
{
}

ServerNS::DirectMessage::~DirectMessage()
{
}

bool ServerNS::DirectMessage::SendToUser(String toUser)
{
	return m_to.compare(toUser) == 0;
}