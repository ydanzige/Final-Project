#include "Message.h"

ServerNS::Message::Message(String from, String to, String content) : m_timestamp(0)
{
	m_from = from;
	m_message = content;
	m_to = to;
}

ServerNS::Message::~Message()
{
}

String ServerNS::Message::GetContent() const
{
	return this->m_message;
}

String ServerNS::Message::GetTimestamp() const
{
	char buff[200];
	tm t_tm;
	localtime_s(&t_tm, &m_timestamp);
	asctime_s(buff, 200, &t_tm);
	std::string str(buff);
	return String(std::wstring(str.begin(), str.end()));
}

String ServerNS::Message::GetFrom() const
{
	return m_from;
}

String ServerNS::Message::GetTo() const
{
	return m_to;
}
