##
## EPITECH PROJECT, 2026
## zappy
## File description:
## Makefile
##

SUBDIRS = ai gui network
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Igui/src -Inetwork/include -I/usr/local/include -I/opt/homebrew/opt/sfml/include
LDFLAGS = -L/usr/local/lib -L/opt/homebrew/opt/sfml/lib -lsfml-graphics -lsfml-window -lsfml-system
CRITERION_FLAGS = $(shell pkg-config --cflags --libs criterion)
TEST_ARTIFACTS = tests/gui_unit tests/network_unit tests/network_functional tests/*.o
TEST_DIRS = tests/__pycache__

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

zappy_ai:
	$(MAKE) -C ai zappy_ai

zappy_gui:
	$(MAKE) -C gui zappy_gui

zappy_server:
	$(MAKE) -C network zappy_server

tests_run:
	@printf "\033[1;36m\n== AI Python tests ==\033[0m\n"
	$(MAKE) ai_tests_run
	@printf "\033[1;36m\n== Network C++ tests ==\033[0m\n"
	$(MAKE) network_tests_run
	@printf "\033[1;36m\n== GUI C++ tests ==\033[0m\n"
	$(MAKE) gui_tests_run

ai_tests_run:
	python3 tests/run_unittest.py

network_tests_run:
	$(MAKE) -C network tests_run

gui_tests_run:
	$(CXX) $(CXXFLAGS) tests/gui_unit.cpp gui/src/client/Client.cpp network/src/utils/Socket.cpp network/src/utils/Poll.cpp -o tests/gui_unit $(LDFLAGS) $(CRITERION_FLAGS)
	./tests/gui_unit

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	rm -f $(TEST_ARTIFACTS)
	rm -rf $(TEST_DIRS)

fclean: clean
	@for target in zappy_ai zappy_gui zappy_server; do \
		rm -f $$target; \
	done
	rm -f $(TEST_ARTIFACTS)
	rm -rf $(TEST_DIRS)

re: fclean all

.PHONY: all clean fclean re zappy_ai zappy_gui zappy_server tests_run ai_tests_run network_tests_run gui_tests_run
