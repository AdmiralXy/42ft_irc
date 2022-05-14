CC = c++
RM = rm -f
# TODO remove -g flag
CFLAGS = -Wall -Wextra -Werror -Wpedantic -std=c++98 -Isrc -I. -g

NAME = ircserv

INCLUDES = Server.h User.h Utility.h

SRCS = main.cpp
OBJS = $(SRCS:%.cpp=%.o)

%.o: %.cpp $(INCLUDES)
	@$(CC) $(CFLAGS) -c $< -o $(<:.cpp=.o)
	@echo "Building: \033[0;34m$<\033[0m"

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "Binary \033[0;32m$(NAME)\033[0m successfully linked!"

all: $(NAME_UNIT_TESTS) $(NAME_BENCHMARK)

clean:
	@$(RM) $(OBJS)
	@echo "Cleanup: \033[0;34mobject files\033[0m"

fclean: clean
	@$(RM) $(NAME)
	@echo "Cleanup: \033[0;34mlinked files\033[0m"

re: clean all

.PHONY: all