#include "Message.h"

ServerNS::Message::Message():m_timestamp(0)
{
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
	return String(buff);
}
