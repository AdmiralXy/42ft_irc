#pragma once

#include "Utility.h"

class Message
{
private:
    std::string _prefix;
    std::string _cmd;
    std::vector<std::string> _parameters;
    std::string _trailing;
public:
    Message(const std::string& input)
	{
		std::vector<std::string> vec;

		vec = split(input, " ");

		std::vector<std::string>::iterator i = vec.begin();

		if (vec.front()[0] == ':')
		{
			_prefix = vec.front();
			++i;
		}
		_prefix.erase(0, 1);

		for (; i != vec.end(); ++i)
		{
			if ((*i)[0] == ':')
				break;
			if ((*i)[0] != ':' && !_cmd.length())
				_cmd = *i;
			else if ((*i)[0] != ':')
				_parameters.push_back(*i);
		}

		for (; i != vec.end(); ++i)
		{
			_trailing += (*i) + " ";
		}
		if (_trailing.length() > 0)
		{
			_trailing.erase(0, 1);
			_trailing.erase(_trailing.length() - 1, 1);
		}
	}

    ~Message() {}

private:
	inline static std::vector<std::string> split(const std::string &input, const char* ch)
	{
		std::vector<std::string> result;

		char *pch = strtok(const_cast<char*>(input.c_str()), ch);
		while (pch != NULL) {
			result.push_back(pch);
			pch = strtok(NULL, ch);
		}
		return result;
	}
};
