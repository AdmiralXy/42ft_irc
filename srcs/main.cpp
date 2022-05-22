#include <iostream>
#include "Server.h"

int main(int argc, char **argv)
{
	int port;
	bool shutdown = false;
	if (argc != 3)
		ft::exception(ERROR_USAGE);
	std::istringstream(argv[1]) >> port;
	if (port < 1024 || port > 49151)
		ft::exception(ERROR_PORT);
	Server server(port, argv[2]);
	server.createSocket();
	server.bindSocket();
	server.listenSocket();
	server.connectBot();
	while (!shutdown)
	{
		server.acceptConnections();
		shutdown = server.receiveMessages();
		server.clearInactive();
	}
	return 0;
}
