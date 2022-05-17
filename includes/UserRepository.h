#pragma once

#include "Utility.h"
#include "User.h"

User *findByNickname(std::vector<User*> &users, const std::string &nickname)
{
	for (std::vector<User*>::iterator iterator = users.begin(); iterator != users.end(); iterator++)
		if ((*iterator)->getNickname() == nickname)
			return *iterator;
	return 0;
}

User *findByUsername(std::vector<User*> &users, const std::string &username)
{
	for (std::vector<User*>::iterator iterator = users.begin(); iterator != users.end(); iterator++)
		if ((*iterator)->getUsername() == username)
			return *iterator;
	return 0;
}

std::string toString(std::vector<User*> &vector)
{
	std::string result;
	std::vector<User*>::iterator it = vector.begin();
	for (; it != vector.end() - 1; it++)
	{
		result += (*it)->getNickname() + " ";
	}
	result += (*it)->getNickname();
	return result;
}
