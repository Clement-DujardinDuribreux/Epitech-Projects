##
## EPITECH PROJECT, 2024
## makefile
## File description:
## makefile
##

SRC 	=	my_strcat.c \
		my_str_islower.c \
		my_strlen.c \
		my_strncpy.c \
		my_getnbr.c \
		my_put_nbr.c \
		my_strcmp.c \
		my_str_isnum.c \
		my_strlowcase.c \
		my_strstr.c \
		my_compute_power_rec.c \
		my_strcpy.c \
		my_strncat.c \
		my_strupcase.c \
		my_revstr.c \
		my_strncmp.c \
		my_str_isalpha.c \
		my_str_isupper.c \
		my_strncmp.c \
		my_str_to_word_array.c \
		free_word_array.c \
		in_word_array.c \
		my_strdup.c \
		int_to_str.c \
		len_word_array.c \
		useful_func.c \

OBJ 	=	$(SRC:.c=.o)

NAME	= 	libmy.a

INCLUDE =	./

CFLAGS =	-I$(INCLUDE)

all: $(NAME)

$(NAME): $(OBJ)
	ar rc $(NAME) $(OBJ)
	cp $(NAME) ../

clean:
	@rm -f $(OBJ)
	@rm -f $(NAME)

fclean: clean
	@rm -f ../$(NAME)

re: fclean all
