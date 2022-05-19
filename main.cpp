#include <iostream>
#include "Server.h"

int main(int argc, char **argv)
{
	if (argc != 3)
		ft::exception(ERROR_USAGE);
	int port;
	std::istringstream(argv[1]) >> port;
	if (port < 1024 || port > 49151)
		ft::exception(ERROR_PORT);
	Server server(port, argv[2]);
	server.createSocket();
	server.bindSocket();
	server.listenSocket();
	while (true)
	{
		server.acceptUsers();
		server.receiveMessages();
		server.clearInactive();
	}
}
