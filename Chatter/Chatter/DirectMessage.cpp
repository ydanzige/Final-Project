#include "DirectMessage.h"

Server::DirectMessage::DirectMessage()
{
}

Server::DirectMessage::~DirectMessage()
{
}

bool Server::DirectMessage::SendToUser(String toUser)
{
	return m_to.compare(toUser) == 0;
}