#pragma once

#include "Utility.h"

class Channel
{
private:
	std::string										_name;
	std::string 									_topic;
	std::size_t										_countUsers;
	std::string										_password;
	std::vector<User*>								_users;
	bool											_hasPassword;
public:
	explicit Channel(const std::string &name) : _name(name), _countUsers(1), _hasPassword(false) {}

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

	size_t getCountUsers() const
	{
		return _countUsers;
	}

	void setCountUsers(size_t count_users)
	{
		_countUsers = count_users;
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

	bool isHasPassword() const
	{
		return _hasPassword;
	}

	void setHasPassword(bool has_password)
	{
		_hasPassword = has_password;
	}
};
