#ifndef MESSAGES_FA2ZE2MV
#define MESSAGES_FA2ZE2MV
#include <string.h>

enum {
	MESSAGE_FLAVOUR,
	MESSAGE_NOTIFICATION,
	MESSAGE_ACTION,
	MESSAGE_WARNING,
};

struct Message
{
	std::string m_message;
	float m_turn;
	int m_importance;
};

class MessageHandler
{
	public:
		MessageHandler();
		void showMessage(std::string message, int importance);
		Message getNthLatestMessage(int n);
	private:
		static const int m_numMaxMessages=200;
		Message m_messages[m_numMaxMessages];
		int m_currMessage;
};


#endif /* end of include guard: MESSAGES_FA2ZE2MV */
