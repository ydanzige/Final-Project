#include "Message.h"

Server::Message::Message():m_timestamp(0)
{
}

Server::Message::~Message()
{
}

String Server::Message::GetContent() const
{
	return this->m_message;
}

String Server::Message::GetTimestamp() const
{
	return std::asctime(std::localtime(&m_timestamp));
}
