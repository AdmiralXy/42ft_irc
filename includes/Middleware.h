#ifndef INC_42FT_IRC_MIDDLEWARE_H
#define INC_42FT_IRC_MIDDLEWARE_H

class Middleware
{
private:
	User &user;
public:
	explicit Middleware(User &user) : user(user) {}

	bool registerAccess()
	{
		return !user.getServerPassword().empty() && !isRegistered();
	}

	bool nickAccess()
	{
		return user.isRegistered();
	}
private:
	bool hasUsername()
	{
		return !user.getUsername().empty();
	}

	bool isRegistered()
	{
		return user.isRegistered();
	}
};

#endif
