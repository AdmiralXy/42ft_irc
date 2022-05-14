#include <iostream>
#include <cstdlib>
#include "Server.h"

int main(int argc, char **argv)
{
	if (argc != 3)
		ftExit(ERROR_USAGE);

	int port;
	std::istringstream(argv[1]) >> port;
	if (port < 1024 || port > 49151)
		ftExit(ERROR_PORT);
	Server server(port, argv[2]);
	server.createSocket();
	server.bindSocket();
	server.listenSocket();
	while (1)
	{
		server.acceptUsers();
		server.receiveMessages();
//		server.deleteNotActiveUsers();
	}
	return 0;
}
