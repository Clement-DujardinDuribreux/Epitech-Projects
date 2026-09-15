##
## EPITECH PROJECT, 2024
## makefile
## File description:
## makefile
##

SRC 	=	add.c \
			del.c \
			apply.c \
			apply_arg.c \
			sort.c \
			list_len.c \
			get_node.c \
			append_linked_list.c \

OBJ 	=	$(SRC:.c=.o)

NAME	= 	liblist.a

INCLUDE =	./

CFLAGS =	-I$(INCLUDE)

all: $(OBJ) $(NAME)

$(NAME):
	ar rc $(NAME) $(OBJ)
	cp $(NAME) ../

clean:
	@rm -f $(OBJ)
	@rm -f $(NAME)

fclean: clean
	@rm ../$(NAME)

re: fclean all
