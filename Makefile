CC = c++
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -Wpedantic -std=c++98 -Isrc -Iincludes

NAME = ircserv

INCLUDES = includes/Channel.h \
			includes/ChannelRepository.h \
			includes/Command.h \
			includes/Constants.h \
			includes/Middleware.h \
			includes/Request.h \
			includes/Server.h \
			includes/User.h \
			includes/UserRepository.h \
			includes/Utility.h

SRCS = srcs/main.cpp
OBJS = $(SRCS:%.cpp=%.o)

%.o: %.cpp $(INCLUDES)
	@$(CC) $(CFLAGS) -c $< -o $(<:.cpp=.o)
	@echo "Building: \033[0;34m$<\033[0m"

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "Binary \033[0;32m$(NAME)\033[0m successfully linked!"

all: $(NAME)

clean:
	@$(RM) $(OBJS)
	@echo "Cleanup: \033[0;34mobject files\033[0m"

fclean: clean
	@$(RM) $(NAME)
	@echo "Cleanup: \033[0;34mlinked files\033[0m"

re: clean all

leaks: all
	valgrind ./$(NAME) 3030 123

.PHONY: all