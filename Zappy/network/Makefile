##
## EPITECH PROJECT, 2026
## The Plazza
## File description:
## Makefile
##

CXX = g++

INCLUDE = 	-Iinclude

CXXFLAGS =  -std=c++20 -Wall -Wextra -Werror
CRITERION_FLAGS = $(shell pkg-config --cflags --libs criterion)

NAME = zappy_server

SRCS = 	src/main.cpp \
		src/Server.cpp \
		src/Client.cpp \
		src/utils/Poll.cpp \
		src/utils/Socket.cpp \
		src/utils/ServerArgs.cpp \
		src/utils/BroadcastDirection.cpp \
		src/utils/TerminalStyle.cpp \
		src/game/Timeline.cpp \
		src/game/Game.cpp \
		src/game/Inventory.cpp \
		src/game/Player.cpp \
		src/game/Square.cpp \
		src/commands/CommandFactory.cpp \
		src/commands/ACommand.cpp \
		
OBJS = $(SRCS:.cpp=.o)

UNIT_NAME = ../tests/network_unit
FUNCTIONAL_NAME = ../tests/network_functional

COMMON_TEST_SRCS = \
		src/Server.cpp \
		src/Client.cpp \
		src/utils/Poll.cpp \
		src/utils/Socket.cpp \
		src/utils/ServerArgs.cpp \
		src/utils/BroadcastDirection.cpp \
		src/utils/TerminalStyle.cpp \
		src/game/Timeline.cpp \
		src/game/Game.cpp \
		src/game/Inventory.cpp \
		src/game/Player.cpp \
		src/game/Square.cpp \
		src/commands/CommandFactory.cpp \
		src/commands/ACommand.cpp

UNIT_SRCS = ../tests/network_unit.cpp $(COMMON_TEST_SRCS)
UNIT_OBJS = $(UNIT_SRCS:.cpp=.o)
FUNCTIONAL_SRCS = ../tests/network_functional.cpp
FUNCTIONAL_OBJS = $(FUNCTIONAL_SRCS:.cpp=.o)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@  $(INCLUDE) -g

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $(NAME) $(CXXFLAGS)
	mv $(NAME) ../

clean:
	rm -f $(OBJS) $(UNIT_OBJS) $(FUNCTIONAL_OBJS)

fclean: clean
	rm -rf ../$(NAME) $(UNIT_NAME) $(FUNCTIONAL_NAME)

re: fclean all

tests_run: $(UNIT_NAME) $(FUNCTIONAL_NAME) $(NAME)
	$(UNIT_NAME)
	$(FUNCTIONAL_NAME)

$(UNIT_NAME): $(UNIT_OBJS)
	$(CXX) $(UNIT_OBJS) -o $(UNIT_NAME) $(CXXFLAGS) $(CRITERION_FLAGS)

$(FUNCTIONAL_NAME): $(FUNCTIONAL_OBJS)
	$(CXX) $(FUNCTIONAL_OBJS) -o $(FUNCTIONAL_NAME) $(CXXFLAGS) $(CRITERION_FLAGS)

.PHONY: all clean fclean re tests_run
