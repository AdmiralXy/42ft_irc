#pragma once

#include <sstream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <netinet/in.h>
#include <csignal>
#include <fcntl.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <vector>

#include "Constants.h"

#define to_string(x) dynamic_cast< std::ostringstream& >((std::ostringstream() << std::dec << x)).str()

namespace ft
{
	std::string format(const char *fmt, ...)
	{
		char *answer;
		va_list arg;
		va_start(arg, fmt);
		vasprintf(&answer, fmt, arg);
		va_end(arg);
		std::string result(answer);
		free(answer);
		return result;
	}

	void exception(const std::string &error)
	{
		std::cout << error << std::endl;
		exit(1);
	}
}

//void ftMessageJoin(const User &user, const std::string &message, std::size_t code = 0, const std::string& channelName = "", bool isMid = false)
//{
//	std::string username = user.getUsername();
//	if (username.empty())
//		username = "<unnamed>";
//	if (isMid)
//		username = username + " =";
//	std::string msg = std::string(":42ft_irc " + to_string(code) + " " + username + " " + channelName + " :" + message + "\r\n");
//	send(user.getSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
//	std::cout << "OUT <- " << msg;
//}
//
//void ftMessage(const User &user, const std::string &message, std::size_t code = 0)
//{
//	std::string username = user.getUsername();
//	if (username.empty())
//		username = "<unnamed>";
//	std::string msg = std::string(":42ft_irc " + to_string(code) + " " + username + " :" + message + "\r\n");
//	send(user.getSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
//	std::cout << "OUT <- " << msg;
//}
//
//void ftSimpleMessage(const User &user, const std::string &message)
//{
//	std::string msg = std::string(message + "\r\n");
//	send(user.getSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
//	std::cout << "OUT <- " << msg;
//}

std::vector<std::string> split(const std::string& s, const std::string& delimiter)
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
		token = s.substr (pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back (token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}
