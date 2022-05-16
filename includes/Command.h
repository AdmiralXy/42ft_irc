#pragma once

#include "Request.h"

class Command
{
private:
	User&					_user;
	std::string				_content;
	std::vector<User*>&		_users;
	std::vector<Channel*>&	_channels;
	const std::string		&_serverPassword;
public:
	Command(User &user, const std::string &content, std::vector<User*>& users, std::vector<Channel*>& channels, const std::string& serverPassword):_user(user), _content(content), _users(users), _channels(channels), _serverPassword(serverPassword) {}

	bool validate(int expected, const char *command, const char *fmt, ...)
	{
		bool result;
		va_list arg;
		va_start(arg, fmt);
		result = std::vsscanf(_content.c_str(), fmt, arg) == expected;
		va_end(arg);
		if (!result)
			Request::reply_461(_user, command);
		return result;
	}

	void execute()
	{
		(void)_users;
		(void)_channels;
		char command[10000];
		char input_fs[10000];
		char input_sc[10000];
		char input_th[10000];
		char input_fh[10000];

		if (std::sscanf(_content.c_str(), "%s", command) == 1)
		{
			std::string cmd(command);

			if (cmd == "PASS" && validate(1, command, "PASS :%s", input_fs))
				handlerPass(input_fs);
			else if (cmd == "USER" && validate(4, command, "USER %s %s %s :%s", input_fs, input_sc, input_th, input_fh))
				handlerUser(input_fs, input_sc, input_th, input_fh);
			else if (cmd == "NICK" && validate(1, command, "NICK %s", input_fs))
				commandNick(input_fs);
	//		else if (std::sscanf(_content.c_str(), "NICK %s", input_fs) == 1)
	//			commandNick(input_fs);
			//else if (std::sscanf(_content.c_str(), "JOIN %s", input_fs) == 1)
			//commandJoin(input_fs);
		}
	}
private:
	void handlerPass(const std::string& password)
	{
		if (_serverPassword != password)
			Request::reply(_user, "464 :Password incorrect");
		else
			_user.setServerPassword(password);
	}

	void handlerUser(const std::string& username, const std::string& hostname, const std::string& servername, const std::string& realname)
	{
		(void)hostname;
		(void)servername;
		if (Middleware(_user).registration())
		{
			_user.setUsername(username);
			_user.setRealname(realname);
			_user.setRegistered(true);
		}
	}

	void commandNick(const std::string& nickname)
	{
		if (Middleware(_user).nick())
		{
			if (!findByNickname(_users, nickname)) {
				_user.setNickname(nickname);
				Request::reply(_user, ft::format("001 %s :Hi, welcome to %s", _user.getNickname().c_str(), SERVER_NAME));
				Request::reply(_user, ft::format("251 %s :There are %d users and 0 services on 1 server",  _user.getNickname().c_str(), _users.size()));
			} else {
				Request::reply(_user, ft::format("433 * %s :Nickname is already in use", nickname.c_str()));
			}
		}
	}


//	void commandJoin(const std::string& name)
//	{
//		if (!Middleware(_user).joinAccess()) {
//			ftMessage(_user, ERR_NOACCESS);
//		} else {
//			Channel *channel = _server.findChannelByName(name);
//			if (!channel) {
//				Channel new_channel(name);
//				channel = &new_channel;
//				channel->addUser(&_user);
//				_server._channels.push_back(*channel);
//			}
//			//:Username!Username@127.0.0.1 JOIN #123
//			ftSimpleMessage(_user, ":" + _user.getUsername() + "!" + _user.getUsername() + "@127.0.0.1 JOIN " + name);
//			ftMessageJoin(_user, "No topic is set", RPL_NOTOPIC, name);
//			ftMessageJoin(_user, toString(_server._channels.back().getUsers()), RPL_NAMREPLY, name, true);
//			ftMessageJoin(_user, "End of NAMES list", RPL_ENDOFNAMES, name);
//			//:42ft_irc RPL_NOTOPIC Admiral #1234 :No topic is set
//			//:42ft_irc RPL_NAMREPLY Admiral #1234 :Admiral TestUsername Username ...
//			//::42ft_irc RPL_ENDOFNAMES Admiral #1234 :End of NAMES list
//		}
//	}
};
