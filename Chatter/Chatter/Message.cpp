#include "Message.h"

Server::Message::Message(String from, String to, String content) : m_timestamp(0)
{
	m_from = from;
	m_message = content;
	m_to = to;
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
	char buff[200];
	tm t_tm;
	localtime_s(&t_tm, &m_timestamp);
	asctime_s(buff, 200, &t_tm);
	std::string str(buff);
	return String(std::wstring(str.begin(), str.end()));
}

String Server::Message::GetFrom() const
{
	return m_from;
}

String Server::Message::GetTo() const
{
	return m_to;
}
