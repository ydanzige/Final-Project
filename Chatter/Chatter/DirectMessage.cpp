#include "DirectMessage.h"

ServerNS::DirectMessage::DirectMessage()
{
}

ServerNS::DirectMessage::~DirectMessage()
{
}

bool ServerNS::DirectMessage::SendToUser(String toUser)
{
	return m_to.compare(toUser) == 0;
}