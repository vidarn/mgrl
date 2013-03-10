#include <iostream>
#include "messages.h"

MessageHandler::MessageHandler()
{
	for(int i=0;i<m_numMaxMessages;i++){
		m_messages[i].m_message = "";
	}
}

void
MessageHandler::showMessage(std::string message, int importance)
{
	Message msg;
	m_messages[m_currMessage].m_message = message;
	m_messages[m_currMessage].m_importance = importance;
	m_currMessage = (m_currMessage+1)%m_numMaxMessages;
	std::cout << message << std::endl;
}

Message
MessageHandler::getNthLatestMessage(int n)
{
	int messageId = m_currMessage - n;
	messageId = messageId >= 0 ? messageId : messageId + m_numMaxMessages-1;
	return m_messages[messageId];
}
