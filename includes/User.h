#pragma once

#include "Utility.h"

class User
{
private:
	int							_socket;
	std::string					_host;
	std::vector<std::string>	_message;
	bool                        _active;
	std::string					_serverPassword;
	std::string					_nickname;
	std::string					_username;
	std::string 				_realname;
	bool						_registered;
	bool						_serverOperator;
public:
	User(int socket, const std::string &host) : _socket(socket), _host(host), _active(true), _registered(false), _serverOperator(false) {}

	int readMessage()
	{
		char		buf[4096];
		std::string	message;

		for(;;) {
			memset(buf, 0, 4096);
			int bytes = recv(_socket, buf, 4095, 0);
			if (bytes > 0)
			{
				buf[bytes] = '\0';
				message += buf;
				if (message == "\n" || message.empty())
				{
					message = "";
					continue;
				}
				std::cout << "IN ->  " << message;
				if (message.find('\n') != std::string::npos) {
					break;
				}
			}
			if (bytes <= 0)
				return (-1);
		}
		std::vector<std::string> vector = ft::split(message, "\r\n");
		_message.insert(_message.begin(), vector.begin(), vector.end());
		return (0);
	}

	int getSocket() const
	{
		return _socket;
	}

	std::vector<std::string> &getMessage()
	{
		return _message;
	}

	bool isActive() const
	{
		return _active;
	}

	void setActive(bool active)
	{
		_active = active;
	}

	const std::string &getServerPassword() const
	{
		return _serverPassword;
	}

	void setServerPassword(const std::string &server_password)
	{
		_serverPassword = server_password;
	}

	const std::string &getNickname() const
	{
		return _nickname;
	}

	void setNickname(const std::string &nickname)
	{
		_nickname = nickname;
	}

	void setUsername(const std::string &username)
	{
		_username = username;
	}

	void setRealname(const std::string &realname)
	{
		_realname = realname;
	}

	bool isRegistered() const
	{
		return _registered;
	}

	void setRegistered(bool registered)
	{
		_registered = registered;
	}

	std::string getPrefix()
	{
		return _nickname + "!" + _username + "@" + _host;
	}

	bool isServerOperator() const
	{
		return _serverOperator;
	}

	void setServerOperator(bool server_operator)
	{
		_serverOperator = server_operator;
	}
};
