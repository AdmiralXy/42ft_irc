#ifndef INC_42FT_IRC_MIDDLEWARE_H
#define INC_42FT_IRC_MIDDLEWARE_H

#include "Utility.h"
#include "Request.h"

class Middleware
{
private:
	User &_user;
public:
	explicit Middleware(User &user) : _user(user) {}

	bool isAuthorized()
	{
		bool condition = !_user.getServerPassword().empty();
		if (!condition)
			Request::reply(_user, "462 :Unauthorized command (server protected by password)");
		return condition;
	}

	bool registration()
	{
		if (!isAuthorized())
			return false;
		bool condition = !_user.isRegistered();
		if (!condition)
			Request::reply(_user, "462 :Unauthorized command (already registered)");
		return condition;
	}

	bool nick()
	{
		if (!isAuthorized())
			return false;
		bool condition = _user.isRegistered();
		if (!condition)
			Request::reply(_user, "462 :Unauthorized command (not registered)");
		return condition;
	}

	bool joinAccess()
	{
		return !_user.getServerPassword().empty() && _user.isRegistered();
	}
};

#endif
