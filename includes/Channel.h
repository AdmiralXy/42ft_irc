#pragma once

#include "Utility.h"
#include "UserRepository.h"
#include "Request.h"

class Channel
{
private:
	std::string										_name;
	std::string 									_topic;
	std::string										_password;
	std::vector<User*>								_users;
	std::string										_operator;
	bool											_inviteOnly;
	std::vector<std::string>						_inviteList;
public:
	explicit Channel(const std::string &name, const std::string& user) : _name(name), _operator(user), _inviteOnly(false) {}

	void messageChannelExceptUser(const std::string& prefix, const std::string& command, const std::string& message, const User& user)
	{
		for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); it++)
			if ((*it)->getNickname() != user.getNickname())
				Request::reply_raw(*(*it), ft::format(":%s %s %s", prefix.c_str(), command.c_str(), message.c_str()));
	}

	void messageChannel(const std::string& prefix, const std::string& command, const std::string& message)
	{
		for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); it++)
			Request::reply_raw(*(*it), ft::format(":%s %s %s", prefix.c_str(), command.c_str(), message.c_str()));
	}

	void messageChannelRaw(const std::string& message)
	{
		for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); it++)
			Request::reply_raw(*(*it), message);
	}

	std::string getNames()
	{
		return usersToString();
	}

	User *getUserByNickname(const std::string &user)
	{
		return findByNickname(_users, user);
	}

	void removeUser(User &user)
	{
		for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); it++)
		{
			if ((*it)->getNickname() == user.getNickname())
			{
				_users.erase(it);
				break;
			}
		}
	}

	bool isEmpty()
	{
		return _users.empty();
	}

	const std::string &getName() const
	{
		return _name;
	}

	const std::string &getTopic() const
	{
		return _topic;
	}

	void setTopic(const std::string &topic)
	{
		_topic = topic;
	}

	void addUser(User *user)
	{
		_users.push_back(user);
	}

	bool isInviteOnly() const
	{
		return _inviteOnly;
	}

	void setInviteOnly(bool invite_only)
	{
		_inviteOnly = invite_only;
	}

	bool isOperator(const User &user) const
	{
		return user.getNickname() == _operator;
	}

	void addToInviteList(const User &user)
	{
		_inviteList.push_back(user.getNickname());
	}

	bool isInInviteList(const User &user)
	{
		for (std::vector<std::string>::iterator it = _inviteList.begin(); it != _inviteList.end(); it++)
			if ((*it) == user.getNickname())
				return true;
		return false;
	}

	void removeFromInviteList(const User &user)
	{
		for (std::vector<std::string>::iterator it = _inviteList.begin(); it != _inviteList.end(); it++)
		{
			if ((*it) == user.getNickname())
			{
				_inviteList.erase(it);
				return;
			}
		}
	}
private:
	std::string usersToString()
	{
		std::string result;
		std::vector<User*>::iterator it = _users.begin();
		for (; it != _users.end() - 1; it++)
		{
			std::string nickname = (*it)->getNickname();
			if (nickname == _operator)
				result += "@" + nickname + " ";
			else
				result += nickname + " ";
		}
		std::string nickname = (*it)->getNickname();
		if (nickname == _operator)
			result += "@" + nickname + " ";
		else
			result += nickname + " ";
		return result;
	}
};
