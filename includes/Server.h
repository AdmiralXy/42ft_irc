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

	bool receiveMessages()
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
								if (command.execute())
									return clearAll();
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
		return false;
	}

	void clearInactive()
	{
		std::vector<User*>::iterator beg = _users.begin();
		std::vector<User*>::iterator end = _users.end();
		while (beg != end)
		{
			if (!(*beg)->isActive())
			{
				for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
				{
					if ((*it)->getUserByNickname((*beg)->getNickname())) {
						(*it)->messageChannel((*beg)->getPrefix(), "PART", ft::format("%s :%s", (*it)->getName().c_str(), (*beg)->getNickname().c_str()));
						(*it)->removeUser(*(*beg));
					}
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
		for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end();)
		{
			if ((*it)->isEmpty()) {
				_channels.erase(it);
				delete *it;
			} else {
				it++;
			}
		}
	}

	bool clearAll()
	{
		for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); it++)
			delete *it;
		for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
			delete *it;
		_users.clear();
		_channels.clear();
		return true;
	}

	void addBot()
	{
		std::cout << "IRC BOT connected..." << std::endl;
		User *user = new User(-1, SERVER_NAME);
		user->setServerOperator(true);
		user->setServerPassword(_password);
		user->setRegistered(true);
		user->setNickname(SERVER_IRC_BOT_NICKNAME);
		user->setUsername(SERVER_IRC_BOT_NICKNAME);
		user->setRealname(SERVER_IRC_BOT_NICKNAME);
		pollfd pfd;
		pfd.fd = -1;
		pfd.events = POLLIN;
		pfd.revents = POLLERR;
		_fdUsers.push_back(pfd);
		_users.push_back(user);
	}
};
