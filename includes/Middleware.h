#pragma once

#include "Utility.h"
#include "Request.h"

class Middleware
{
private:
	User &_user;
public:
	explicit Middleware(User &user) : _user(user) {}

	bool registration()
	{
		if (!isRegistered())
			return true;
		Request::reply(_user, "462 :Unauthorized command (already registered)");
		return false;
	}

	bool nick()
	{
		if (!isAuthorized()) {
			Request::reply(_user, "462 :Unauthorized command (server protected by password)");
			return false;
		}
		if (!isRegistered()) {
			Request::reply(_user, "462 :Unauthorized command (not registered)");
			return false;
		}
		return true;
	}

	bool join()
	{
		if (!isAuthorized()) {
			Request::reply(_user, "462 :Unauthorized command (server protected by password)");
			return false;
		}
		if (!isRegistered()) {
			Request::reply(_user, "462 :Unauthorized command (not registered)");
			return false;
		}
		return true;
	}

private:
	bool isAuthorized()
	{
		return !_user.getServerPassword().empty();
	}

	bool isRegistered()
	{
		return _user.isRegistered();
	}
};
