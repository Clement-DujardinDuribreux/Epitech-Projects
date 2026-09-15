SRC =   main.cpp \
        src/client/Client.cpp \
        ../network/src/utils/Socket.cpp \
        ../network/src/utils/Poll.cpp \

OBJ =   $(SRC:.cpp=.o)

NAME        =   zappy_gui
CXX         =   g++
CXXFLAGS    =   -std=c++17 -Wall -Wextra -I./src -I../network/include -I/usr/local/include -I/opt/homebrew/opt/sfml/include

all:    $(NAME)

$(NAME):    $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ) -L/usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system -L/opt/homebrew/opt/sfml/lib -L/usr/local/lib
	mv $(NAME) ../

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f ../$(NAME)

re: fclean all
