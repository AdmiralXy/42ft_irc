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
	bool											_isInviteOnly;
	bool											_isTopicOperator;
public:
	explicit Channel(const std::string &name) : _name(name), _isInviteOnly(false), _isTopicOperator(false) {}

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

	std::string getNames()
	{
		return toString(_users);
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

	void setName(const std::string &name)
	{
		_name = name;
	}

	const std::string &getTopic() const
	{
		return _topic;
	}

	void setTopic(const std::string &topic)
	{
		_topic = topic;
	}

	const std::string &getPassword() const
	{
		return _password;
	}

	void setPassword(const std::string &password)
	{
		_password = password;
	}

	void addUser(User *user)
	{
		_users.push_back(user);
	}

	std::vector<User*> &getUsers()
	{
		return _users;
	}

	void setUsers(const std::vector<User*> &users)
	{
		_users = users;
	}

	bool isIsInviteOnly() const
	{
		return _isInviteOnly;
	}

	void setIsInviteOnly(bool is_invite_only)
	{
		_isInviteOnly = is_invite_only;
	}

	bool isIsTopicOperator() const
	{
		return _isTopicOperator;
	}

	void setIsTopicOperator(bool is_topic_operator)
	{
		_isTopicOperator = is_topic_operator;
	}
};
