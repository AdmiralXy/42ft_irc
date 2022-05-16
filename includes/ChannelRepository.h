#pragma once

#include "Utility.h"
#include "Channel.h"

Channel *findByName(std::vector<Channel*> &channels, const std::string &name)
{
	for (std::vector<Channel*>::iterator iterator = channels.begin(); iterator != channels.end(); iterator++)
		if ((*iterator)->getName() == name)
			return *iterator;
	return 0;
}
