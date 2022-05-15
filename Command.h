#ifndef INC_42FT_IRC_COMMAND_H
#define INC_42FT_IRC_COMMAND_H

class Command
{
private:
	std::string content;
public:
	Command(const std::string &content) : content(content) {}

	void identify()
	{
		char command[1000];

		std::cout << content.c_str() << std::endl;
		if (std::sscanf(content.c_str(), "PASS :%s", command) == 1)
			std::cout << "Command is: " << command << std::endl;
	}
};

#endif
