#ifndef INC_42FT_IRC_CHANNEL_H
#define INC_42FT_IRC_CHANNEL_H

#include "Utility.h"

class Channel
{
private:
	std::string										_name;
	std::size_t										_countUsers;
	std::string										_pass;
	std::vector<std::pair <std::string, int> >		_users;
	bool											_hasPass;
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

	const std::string &getPass() const
	{
		return _pass;
	}

	void setPass(const std::string &pass)
	{
		_pass = pass;
	}

	const std::vector<std::pair<std::string, int> > &getUsers() const
	{
		return _users;
	}

	void setUsers(const std::vector<std::pair<std::string, int> > &users)
	{
		_users = users;
	}

	bool isHasPass() const
	{
		return _hasPass;
	}

	void setHasPass(bool has_pass)
	{
		_hasPass = has_pass;
	}
};

#endif
