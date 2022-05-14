#ifndef INC_42FT_IRC_USER_H
#define INC_42FT_IRC_USER_H

#include "Utility.h"

class User
{
private:
	int							_socket;
	std::string					_host;
	std::vector<std::string>	_message;
	bool                        _active;
public:
	User(int socket, const std::string &host) : _socket(socket), _host(host) {}
	int readMessage()
	{
		char buf[4096];
		std::string msg;

		for(;;) {
			memset(buf, 0, 4096);
			int bytesRecv = recv(_socket, buf, 4096, 0);
			if (bytesRecv > 0) {
				buf[bytesRecv] = 0;
				msg += buf;
				if (msg == "\n" || msg == "")
				{
					msg = "";
					continue;
				}
				std::cout << msg;
				if (msg.find('\n') != std::string::npos) {
					if (msg[0] != '\n')
						msg.erase(msg.find('\n'), 1);
					break;
				}
			}
			if (bytesRecv <= 0)
				return (-1);
		}
		_message.push_back(msg);
		return (0);
	}

	const std::vector<std::string> &getMessage() const
	{
		return _message;
	}

	void setMessage(const std::vector<std::string> &message)
	{
		_message = message;
	}

	bool isActive() const
	{
		return _active;
	}

	void setActive(bool active)
	{
		_active = active;
	}
};

#endif
