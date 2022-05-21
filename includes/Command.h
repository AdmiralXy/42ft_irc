#pragma once

#include "Request.h"
#include "ChannelRepository.h"

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

	bool execute()
	{
		bool isSquit = false;

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
			else if (cmd == "USER" && validate(4, command, "USER %s %s %s :%[^\t\n]", input_fs, input_sc, input_th, input_fh))
				handlerUser(input_fs, input_sc, input_th, input_fh);
			else if (cmd == "NICK" && validate(1, command, "NICK %s", input_fs))
				handlerNick(input_fs);
			else if (cmd == "JOIN" && validate(1, command, "JOIN %s", input_fs))
				handlerJoin(input_fs);
			else if (cmd == "PART" && validate(1, command, "PART %s", input_fs))
				handlerPart(input_fs);
			else if (cmd == "PRIVMSG" && validate(2, command, "PRIVMSG %s :%[^\t\n]", input_fs, input_sc))
				handlerNoticePrivmsg(input_fs, input_sc, "PRIVMSG");
			else if (cmd == "NOTICE" && validate(2, command, "NOTICE %s :%[^\t\n]", input_fs, input_sc))
				handlerNoticePrivmsg(input_fs, input_sc, "NOTICE");
			else if (cmd == "KICK" && validate(2, command, "KICK %s %[^\t\n]", input_fs, input_sc))
				handlerKick(input_fs, input_sc);
			else if (cmd == "MODE" && validate(2, command, "MODE %s %s", input_fs, input_sc))
				handlerMode(input_fs, input_sc);
			else if (cmd == "OPER" && validate(2, command, "OPER %s %[^\t\n]", input_fs, input_sc))
				handlerOper(input_fs, input_sc);
			else if (cmd == "SQUIT" && validate(2, command, "SQUIT %s %[^\t\n]", input_fs, input_sc))
				isSquit = handlerSquit(input_fs, input_sc);
			else if (cmd == "INVITE" && validate(2, command, "INVITE %s %s", input_fs, input_sc))
				handlerInvite(input_fs, input_sc);
			else if (cmd == "TOPIC" && validate(2, command, "TOPIC %s :%[^\t\n]", input_fs, input_sc))
				handlerTopic(input_fs, input_sc);
			else
				handlerDefault(command);
		}
		return isSquit;
	}

	void handlerDefault(const std::string& command)
	{
		if (Middleware(_user).join())
			Request::reply(_user, ft::format("461 %s %s :Unknown command", _user.getNickname().c_str(), command.c_str()));
	}

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

	void handlerNick(const std::string& nickname)
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

	void handlerJoin(const std::string& channelName)
	{
		if (Middleware(_user).join())
		{
			Channel *channel = findByName(_channels, channelName);
			if (!channel) {
				channel = new Channel(channelName, _user.getNickname());
				_channels.push_back(channel);
			}
			if (channel->getUserByNickname(_user.getNickname()) || (channel->isInviteOnly() && !channel->isInInviteList(_user)))
				return;
			channel->addUser(&_user);
			channel->messageChannel(_user.getPrefix(), "JOIN", channelName);
			if (!channel->getTopic().empty())
				Request::reply(_user, ft::format("332 %s %s :%s", _user.getNickname().c_str(), channelName.c_str(), channel->getTopic().c_str()));
			else
				Request::reply(_user, ft::format("331 %s %s :No topic is set", _user.getNickname().c_str(), channelName.c_str()));
			Request::reply_raw(_user, ft::format(":%s 353 %s = %s :%s", SERVER_NAME, _user.getNickname().c_str(), channelName.c_str(), channel->getNames().c_str()));
			Request::reply(_user, ft::format("366 %s %s :End of NAMES list", _user.getNickname().c_str(), channelName.c_str()));
		}
	}

	void handlerPart(const std::string& channelName)
	{
		if (Middleware(_user).join())
		{
			Channel *channel = findByName(_channels, channelName);
			if (!channel || !channel->getUserByNickname(_user.getNickname())) {
				Request::reply(_user, ft::format("442 %s %s :You're not on that channel", _user.getNickname().c_str(), channelName.c_str()));
				return;
			}
			channel->messageChannel(_user.getPrefix(), "PART", ft::format("%s :%s left", channelName.c_str(), _user.getNickname().c_str()));
			channel->removeUser(_user);
		}
	}

	void handlerNoticePrivmsg(const std::string& target, const std::string& message, const std::string& command)
	{
		if (Middleware(_user).join())
		{
			User *user = findByNickname(_users, target);
			Channel *channel = findByName(_channels, target);

			if (user) {
				Request::reply_raw(*user, ft::format(":%s %s %s :%s", _user.getPrefix().c_str(), command.c_str(), target.c_str(), message.c_str()));
			} else if (channel && channel->getUserByNickname(_user.getNickname())) {
				channel->messageChannelExceptUser(_user.getPrefix(), command, ft::format("%s %s", target.c_str(), message.c_str()), _user);
				botProcess(channel, _user, message, target, command);
			} else if (command != "NOTICE") {
				Request::reply(_user, ft::format("401 %s %s :No such nick/channel", _user.getNickname().c_str(), target.c_str()));
			}
		}
	}

	void handlerKick(const std::string& channelName, const std::string& nickname)
	{
		if (Middleware(_user).nick())
		{
			User *user = findByNickname(_users, nickname);
			Channel *channel = findByName(_channels, channelName);

			if (!user || !channel) {
				Request::reply(_user, ft::format("401 %s %s :No such nick/channel", _user.getNickname().c_str(), nickname.c_str()));
			} else if (!channel->isOperator(_user)) {
				Request::reply(_user, ft::format("%s :You're not channel operator", channelName.c_str()));
			} else {
				channel->messageChannel(user->getPrefix(), "PART", ft::format("%s :%s kicked by channel operator", channelName.c_str(), user->getNickname().c_str()));
				channel->removeUser(*user);
				channel->removeFromInviteList(*user);
			}
		}
	}

	void handlerMode(const std::string& channelName, const std::string& mode)
	{
		if (Middleware(_user).nick())
		{
			Channel *channel = findByName(_channels, channelName);

			if (!channel) {
				Request::reply(_user, ft::format("403 %s %s :No such channel", _user.getNickname().c_str(), channelName.c_str()));
			} else if (!channel->isOperator(_user)) {
				Request::reply(_user, ft::format("%s :You're not channel operator", channelName.c_str()));
			} else if (mode == "+i") {
				channel->setInviteOnly(true);
				channel->messageChannelRaw(ft::format(":%s 221 %s :+i", SERVER_NAME, channelName.c_str()));
			} else if (mode == "-i") {
				channel->setInviteOnly(false);
				channel->messageChannelRaw(ft::format(":%s 221 %s :-i", SERVER_NAME, channelName.c_str()));
			} else {
				Request::reply(_user, ft::format("472 %s %s :is unknown mode char to me", _user.getNickname().c_str(), mode.c_str()));
			}
		}
	}

	void handlerOper(const std::string& nickname, const std::string& password)
	{
		if (Middleware(_user).nick())
		{
			User *user = findByNickname(_users, nickname);

			if (!user) {
				Request::reply(_user, ft::format("401 %s %s :No such nick/channel", _user.getNickname().c_str(), nickname.c_str()));
			} else if (password != SERVER_OPERATOR_PASSWORD) {
				_user.setServerOperator(false);
				Request::reply(_user, "464 :Password incorrect");
			} else {
				_user.setServerOperator(true);
				Request::reply(_user, "381 :You are now an IRC operator");
			}
		}
	}

	bool handlerSquit(const std::string& server, const std::string& comment)
	{
		(void)comment;
		if (!Middleware(_user).serverOperator())
			return false;
		if (server != SERVER_NAME) {
			Request::reply(_user, "402 :No such server");
			return false;
		}
		return true;
	}

	void handlerInvite(const std::string& nickname, const std::string& channelName)
	{
		// TODO mb reply to invited user?
		User *user = findByNickname(_users, nickname);
		Channel *channel = findByName(_channels, channelName);

		if (!user || !channel) {
			Request::reply(_user, ft::format("401 %s %s :No such nick/channel", _user.getNickname().c_str(), nickname.c_str()));
		} else if (channel->isInviteOnly() && !channel->isOperator(_user)) {
			Request::reply(_user, ft::format("%s :You're not channel operator", channelName.c_str()));
		} else {
			channel->addToInviteList(*user);
			Request::reply(_user, ft::format("%s %s", channelName.c_str(), _user.getNickname().c_str()));
			if (user->getNickname() == SERVER_IRC_BOT_NICKNAME) {
				channel->addUser(user);
				channel->messageChannel(user->getPrefix(), "JOIN", channelName);
			}
		}
	}

	void handlerTopic(const std::string& channelName, const std::string& topic)
	{
		if (Middleware(_user).nick())
		{
			Channel *channel = findByName(_channels, channelName);

			if (!channel) {
				Request::reply(_user, ft::format("403 %s %s :No such channel", _user.getNickname().c_str(), channelName.c_str()));
			} else if (!channel->isOperator(_user)) {
				Request::reply(_user, ft::format("%s :You're not channel operator", channelName.c_str()));
			} else {
				channel->messageChannel(_user.getPrefix(), "TOPIC", ft::format("%s :%s", channelName.c_str(), topic.c_str()));
				channel->setTopic(topic);
			}
		}
	}
private:
	inline void botProcess(Channel *channel, const User& from, const std::string& message, const std::string& target, const std::string& command)
	{
		User *bot = channel->getUserByNickname(SERVER_IRC_BOT_NICKNAME);
		if (bot)
		{
			if (message == "Bot, what time is it?") {
				channel->messageChannelExceptUser(bot->getPrefix(), command, ft::format("%s %s", target.c_str(), ft::currentDateTime().c_str()), *bot);
			} else if (message == "Bot, roll") {
				int rand = std::rand() % 101;
				std::string result = from.getNickname() + ", you get " + ft::toString(rand) + "/100.";
				channel->messageChannelExceptUser(bot->getPrefix(), command, ft::format("%s %s", target.c_str(), result.c_str()), *bot);
			} else if (message.rfind("Bot, ", 0) != std::string::npos && message.find('?') != std::string::npos) {
				int rand = std::rand() % 3;
				if (rand == 0)
					channel->messageChannelExceptUser(bot->getPrefix(), command, ft::format("%s %s", target.c_str(), "Yes."), *bot);
				else if (rand == 1)
					channel->messageChannelExceptUser(bot->getPrefix(), command, ft::format("%s %s", target.c_str(), "No."), *bot);
				else if (rand == 2)
					channel->messageChannelExceptUser(bot->getPrefix(), command, ft::format("%s %s", target.c_str(), "I don't know."), *bot);
			}
		}
	}
};
