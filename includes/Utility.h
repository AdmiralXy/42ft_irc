#pragma once

#define ERROR_USAGE "usage: ./ircserv <port> <password>"
#define ERROR_SOCKET "Unable to create socket!"
#define ERROR_PORT "Specified port cannot be used!"
#define ERROR_SOCKET_SET "Unable to set socket!"
#define ERROR_IP_BIND "Unable to bind IP or specified port!"
#define ERROR_SOCKET_LISTEN "Unable to start listening on socket!"

#define ERR_PASSINCORRECT ":Password incorrect\r\n"
#define ERR_NEEDMOREPARAMS ":Need more parameters\r\n"
#define ERR_ALREADYREGISTRED ":Already registered\r\n"
#define ERR_NONICKNAMEGIVEN ":No nickname given\r\n"
#define ERR_NICKCOLLISION ":Nickname is already in use\r\n"

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

void ftExit(const std::string &error)
{
	std::cout << error << std::endl;
	exit(1);
}

void ftError(int _socket, const std::string &error)
{
	send(_socket, error.c_str(), error.size(), MSG_NOSIGNAL);
}

#endif
