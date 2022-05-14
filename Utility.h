#ifndef INC_42FT_IRC_UTILITY_H
#define INC_42FT_IRC_UTILITY_H

#define ERROR_USAGE "usage: ./ircserv <port> <password>"
#define ERROR_SOCKET "Unable to create socket!"
#define ERROR_PORT "Specified port cannot be used!"
#define ERROR_SOCKET_SET "Unable to set socket!"
#define ERROR_IP_BIND "Unable to bind IP or specified port!"
#define ERROR_SOCKET_LISTEN "Unable to start listening on socket!"

#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <sstream>
#include <string>
#include <netinet/in.h>
#include <csignal>
#include <fcntl.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <vector>

void ftExit(std::string error)
{
	std::cout << error << std::endl;
	exit(1);
}

#endif
