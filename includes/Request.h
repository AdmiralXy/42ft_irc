#pragma once

#include "UserRepository.h"
#include "Utility.h"

class Request
{
private:
	static int proceed(User& user, const std::string& message)
	{
		std::cout << "OUT <- " << message;
		return send(user.getSocket(), message.c_str(), message.size(), MSG_NOSIGNAL);
	}
public:
	static int reply(User& user, const std::string& message)
	{
		std::string content = ft::format(":%s %s\r\n", SERVER_NAME, message.c_str());
		return proceed(user, content);
	}

	static int reply_raw(User& user, const std::string& message)
	{
		std::string content = ft::format("%s\r\n", message.c_str());
		return proceed(user, content);
	}

	static int reply_461(User& user, const std::string& command)
	{
		std::string content = ft::format("461 %s %s :Not enough parameters", user.getNickname().c_str(), command.c_str());
		return reply(user, content);
	}
};
