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
