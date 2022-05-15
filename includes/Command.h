#ifndef INC_42FT_IRC_COMMAND_H
#define INC_42FT_IRC_COMMAND_H

#include "Utility.h"
#include "User.h"

class Command
{
private:
	User &_user;
	const std::string &_serverPassword;
	std::string _content;
public:
	Command(User &user, const std::string &server_password, const std::string &content)  : _user(user), _serverPassword(server_password), _content(content) {}

	void identify()
	{
		char input[1000];

		if (std::sscanf(_content.c_str(), "PASS :%s", input) == 1)
			commandPass(input);
		else if (std::sscanf(_content.c_str(), "NICK %s", input) == 1)
			commandNick(input);
	}
private:
	void commandPass(const std::string& password)
	{
		_user.setServerPassword(password);
		if (_serverPassword != _user.getServerPassword())
			ftError(_user.getSocket(), ERR_PASSINCORRECT);
	}

	void commandNick(const std::string& nickname)
	{
		_user.setNickname(nickname);
		if (_serverPassword != _user.getServerPassword())
			ftError(_user.getSocket(), ERR_PASSINCORRECT);
	}
};

#endif
