#pragma once

#include "Utility.h"
#include "User.h"
#include "Channel.h"
#include "Middleware.h"

class Server
{
public:
	class Command
	{
	private:
		Server &_server;
		User &_user;
		std::string _content;
	public:
		Command(Server &server, User &user, const std::string &content) : _server(server), _user(user), _content(content) {}

		void identify()
		{
			char input_fs[1000];
			char input_sc[1000];
			char input_th[1000];
			char input_fh[1000];

			if (std::sscanf(_content.c_str(), "PASS :%s", input_fs) == 1)
				commandPass(input_fs);
			else if (std::sscanf(_content.c_str(), "NICK %s", input_fs) == 1)
				commandNick(input_fs);
			else if (std::sscanf(_content.c_str(), "USER %s %s %s :%s", input_fs, input_sc, input_th, input_fh) == 4)
				commandUser(input_fs, input_sc, input_th, input_fh);
		}
	private:
		void commandPass(const std::string& password)
		{
			std::cout << "Command pass" << std::endl;
			if (_server.getPassword() != password)
				ftMessage(_user, ERR_PASSINCORRECT);
			else
				_user.setServerPassword(password);
		}

		void commandNick(const std::string& nickname)
		{
			std::cout << "Command nick" << std::endl;
			if (!Middleware(_user).nickAccess())
				ftMessage(_user, ERR_NOACCESS);
			else
			{
				if (!_server.findByNickname(nickname)) {
					_user.setNickname(nickname);
					ftMessage(_user, "There are " + to_string(_server._users.size()) + " users and 0 services on 1 server", RPL_LUSERCLIENT);
					ftMessage(_user,SUCCESS_REGISTER);
				}
				else
					ftMessage(_user, ERR_NICKCOLLISION);
			}
		}

		void commandUser(const std::string& username, const std::string& hostname, const std::string& servername, const std::string& realname)
		{
			std::cout << "Command user" << std::endl;
			(void)hostname;
			(void)servername;
			(void)realname;
			if (!Middleware(_user).registerAccess()) {
				ftMessage(_user, ERR_NOACCESS);
			} else {
				if (!_server.findByUsername(username)) {
					_user.setUsername(username);
					_user.setRegistered(true);
				} else {
					ftMessage(_user, ERR_ALREADYREGISTRED);
				}
			}
		}
	};
private:
	int						_port;
	std::string				_password;
	int						_listening;
	sockaddr_in				_sockaddr;
	int						_clientSocket;
	std::vector<pollfd>		_fdUsers;
	std::vector<User *>		_users;
	std::vector<Channel *>	_channels;
public:
	Server(int port, const std::string &password) : _port(port), _password(password), _listening(), _sockaddr(), _clientSocket() {}

	void createSocket()
	{
		_listening = socket(AF_INET, SOCK_STREAM, 0);
		if (_listening == -1)
			ftExit(ERROR_SOCKET);
		(void)_port;
	}

	void bindSocket()
	{
		int optval = 1;
		_sockaddr.sin_family = AF_INET;
		_sockaddr.sin_port = htons(_port);
		_sockaddr.sin_addr.s_addr = 0UL;
		if ((setsockopt(_listening, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int))) == -1) {
			close(_listening);
			ftExit(ERROR_SOCKET_SET);
		}
		if (bind(_listening, (struct sockaddr *)&_sockaddr, sizeof(_sockaddr)) < 0) {
			close(_listening);
			ftExit(ERROR_IP_BIND);
		}
	}

	void listenSocket() const
	{
		if (listen(_listening, SOMAXCONN) == -1) {
			close(_listening);
			ftExit(ERROR_SOCKET_LISTEN);
		}
		fcntl(_listening, F_SETFL, O_NONBLOCK);
	}

	void acceptUsers()
	{
		sockaddr_in	client;
		socklen_t clientSize = sizeof(client);
		_clientSocket = accept(_listening, (sockaddr *)&client, &clientSize);
		if (_clientSocket >= 0) {
			char host[INET_ADDRSTRLEN];
			memset(host, 0, INET_ADDRSTRLEN);
			inet_ntop(AF_INET, &(client.sin_addr), host, INET_ADDRSTRLEN);
			std::cout << host << " connect on " << ntohs(client.sin_port) << std::endl;
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
		if (ret > 0) {
			std::vector<pollfd>::iterator it = _fdUsers.begin();
			std::vector<pollfd>::iterator it2 = _fdUsers.end();
			for (; it != it2; ++it) {
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
								Command command(*this,*_users[idx], messages.front());
								command.identify();
								messages.erase(messages.begin());
							}
						}
						catch (const std::exception & ex)
						{
							send(_users[idx]->getSocket(), ex.what(), std::string(ex.what()).size(), MSG_NOSIGNAL);
						}
					}
				}
				it->revents = 0; // reset
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
				for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
				{
					// Очистка из каналов
					//(*it)->eraseUserForChannel((*beg)->getNickName(), (*beg)->getSocket());
					//(*it)->eraseUserForInvaiteList((*beg)->getNickName());
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

	const std::string &getPassword() const
	{
		return _password;
	}

	const std::vector<User*> &getUsers() const
	{
		return _users;
	}

	void setUsers(const std::vector<User*> &users)
	{
		_users = users;
	}

	User *findByNickname(const std::string &nickname)
	{
		for (std::vector<User*>::iterator iterator = _users.begin(); iterator != _users.end(); iterator++)
			if ((*iterator)->getNickname() == nickname)
				return *iterator;
		return 0;
	}

	User *findByUsername(const std::string &username)
	{
		for (std::vector<User*>::iterator iterator = _users.begin(); iterator != _users.end(); iterator++)
			if ((*iterator)->getUsername() == username)
				return *iterator;
		return 0;
	}
};
