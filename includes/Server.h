#pragma once

#include "Utility.h"
#include "User.h"
#include "Channel.h"
#include "Middleware.h"
#include "Command.h"

class Server
{
private:
	int						_port;
	std::string				_password;
	int						_listening;
	sockaddr_in				_sockaddr;
	int						_clientSocket;
	std::vector<pollfd>		_fdUsers;
	std::vector<User*>		_users;
	std::vector<Channel*>	_channels;
public:
	Server(int port, const std::string &password) : _port(port), _password(password), _listening(), _sockaddr(), _clientSocket() {}

	void createSocket()
	{
		_listening = socket(AF_INET, SOCK_STREAM, 0);
		if (_listening == -1)
			ft::exception(ERROR_SOCKET);
		std::cout << "Listening on port: " << _port << std::endl;
	}

	void bindSocket()
	{
		int optval = 1;
		_sockaddr.sin_family = AF_INET;
		_sockaddr.sin_port = htons(_port);
		_sockaddr.sin_addr.s_addr = 0UL;
		if ((setsockopt(_listening, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int))) == -1) {
			close(_listening);
			ft::exception(ERROR_SOCKET_SET);
		}
		if (bind(_listening, (struct sockaddr *)&_sockaddr, sizeof(_sockaddr)) < 0) {
			close(_listening);
			ft::exception(ERROR_IP_BIND);
		}
	}

	void listenSocket() const
	{
		if (listen(_listening, SOMAXCONN) == -1) {
			close(_listening);
			ft::exception(ERROR_SOCKET_LISTEN);
		}
		fcntl(_listening, F_SETFL, O_NONBLOCK);
	}

	void acceptUsers()
	{
		sockaddr_in	client;
		socklen_t clientSize = sizeof(client);
		_clientSocket = accept(_listening, (sockaddr *)&client, &clientSize);
		if (_clientSocket >= 0)
		{
			char host[INET_ADDRSTRLEN];
			memset(host, 0, INET_ADDRSTRLEN);
			inet_ntop(AF_INET, &(client.sin_addr), host, INET_ADDRSTRLEN);
			std::cout << "Accepted connection from " << host << ":" << ntohs(client.sin_port) << std::endl;
			pollfd pfd;
			pfd.fd = _clientSocket;
			pfd.events = POLLIN;
			pfd.revents = POLLERR;
			_fdUsers.push_back(pfd);
			User *user = new User(_clientSocket, host);
			_users.push_back(user);
		}
	}

	void receiveMessages()
	{
		int ret = poll(_fdUsers.data(), _fdUsers.size(), 1000);
		if (ret > 0)
		{
			std::vector<pollfd>::iterator it = _fdUsers.begin();
			std::vector<pollfd>::iterator it2 = _fdUsers.end();
			for (; it != it2; ++it)
			{
				if (it->revents == POLLIN)
				{
					int idx = it - _fdUsers.begin();
					if (_users[idx]->readMessage() == -1)
					{
						_users[idx]->setActive(false);
					}
					else
					{
						try
						{
							std::vector<std::string> &messages = _users[idx]->getMessage();
							while (!messages.empty())
							{
								Command command(*_users[idx], messages.front(), _users, _channels, _password);
								command.execute();
								messages.erase(messages.begin());
							}
						}
						catch (const std::exception & ex)
						{
							send(_users[idx]->getSocket(), ex.what(), std::string(ex.what()).size(), MSG_NOSIGNAL);
						}
					}
				}
				it->revents = 0;
			}
		}
	}

	void clearInactiveUsers()
	{
		std::vector<User*>::iterator beg = _users.begin();
		std::vector<User*>::iterator end = _users.end();
		while (beg != end)
		{
			if (!(*beg)->isActive())
			{
				for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
				{
					// Clear user from all of channels
				}
				close((*beg)->getSocket());
				delete *beg;
				_users.erase(beg);
				int i = beg - _users.begin();
				_fdUsers.erase(_fdUsers.begin() + i);
				break;
			}
			else
				++beg;
		}
	}
};
