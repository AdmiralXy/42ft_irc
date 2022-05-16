#pragma once

#define SERVER_NAME "42ft_irc"

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
