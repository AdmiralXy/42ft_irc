#pragma once

#include "Utility.h"

class Channel
{
private:
	std::string										_name;
	std::size_t										_countUsers;
	std::string										_password;
	std::vector<std::pair <std::string, int> >		_users;
	bool											_hasPassword;
public:
	const std::string &getName() const
	{
		return _name;
	}

	void setName(const std::string &name)
	{
		_name = name;
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

	const std::vector<std::pair<std::string, int> > &getUsers() const
	{
		return _users;
	}

	void setUsers(const std::vector<std::pair<std::string, int> > &users)
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

#endif
