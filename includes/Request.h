#ifndef INC_42FT_IRC_REQUEST_H
#define INC_42FT_IRC_REQUEST_H

#include "UserRepository.h"
#include "Utility.h"

class Request
{
private:
	static int proceed(const User& user, const std::string& message)
	{
		std::cout << "OUT <- " << message;
		return send(user.getSocket(), message.c_str(), message.size(), MSG_NOSIGNAL);
	}
public:
	static int reply(const User& user, const std::string& message)
	{
		std::string content = ft::format(":%s %s\r\n", SERVER_NAME, message.c_str());
		return proceed(user, content);
	}

	static int reply_format(const User& user, const char *fmt, ...)
	{
		va_list arg;
		va_start(arg, fmt);
		std::string content = ft::format(fmt, arg);
		va_end(arg);
		return reply(user, content);
	}

	static int reply_461(const User& user, const std::string& command)
	{
		std::string content = ft::format("461 %s %s :Not enough parameters\r\n", user.getNickname().c_str(), command.c_str());
		return proceed(user, content);
	}
};

#endif
