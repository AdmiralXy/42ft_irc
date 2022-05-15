cmake_minimum_required(VERSION 3.18)
project(42ft_irc)

set(CMAKE_CXX_STANDARD 98)

add_compile_options(-Wall -Wextra -pedantic -Werror -Iincludes)

include_directories(includes)

add_executable(42ft_irc main.cpp includes/Server.h includes/Utility.h includes/User.h includes/Channel.h)
