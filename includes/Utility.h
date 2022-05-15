#pragma once

#define ERROR_USAGE "usage: ./ircserv <port> <password>"
#define ERROR_SOCKET "Unable to create socket!"
#define ERROR_PORT "Specified port cannot be used!"
#define ERROR_SOCKET_SET "Unable to set socket!"
#define ERROR_IP_BIND "Unable to bind IP or specified port!"
#define ERROR_SOCKET_LISTEN "Unable to start listening on socket!"

#define ERR_PASSINCORRECT "Password incorrect"
#define ERR_NEEDMOREPARAMS "Need more parameters"
#define ERR_ALREADYREGISTRED "Already registered"
#define ERR_NONICKNAMEGIVEN "No nickname given"
#define ERR_NICKCOLLISION "Nickname is already in use"
#define ERR_NOACCESS "You have no rights"

#define SUCCESS_REGISTER "Hi, welcome to 42ft_irc server!"

#define RPL_LUSERCLIENT 251

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
#include "User.h"

#define to_string(x) dynamic_cast< std::ostringstream& >((std::ostringstream() << std::dec << x)).str()

void ftExit(const std::string &error)
{
	std::cout << error << std::endl;
	exit(1);
}

void ftMessage(const User &user, const std::string &message, std::size_t code = 0)
{
	std::string username = user.getUsername();
	if (username.empty())
		username = "<unnamed>";
	std::string msg = std::string(":42ft_irc " + to_string(code) + " " + username + " :" + message + "\r\n");
	send(user.getSocket(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
}
