##
## EPITECH PROJECT, 2024
## makefile
## File description:
## makefile
##

SRC     =   src/main.c \
			src/op.c \
			src/flags.c \
			src/get_param.c \
			src/free_champs.c \
			src/reset.c \
			src/instructions/live_cor.c \
			src/instructions/ld_cor.c \
			src/instructions/st_cor.c \
			src/instructions/add_cor.c \
			src/instructions/sub_cor.c \
			src/instructions/and_cor.c \
			src/instructions/or_cor.c \
			src/instructions/xor_cor.c \
			src/instructions/zjmp_cor.c \
			src/instructions/ldi_cor.c \
			src/instructions/sti_cor.c \
			src/instructions/fork_cor.c \
			src/instructions/lld_cor.c \
			src/instructions/lldi_cor.c \
			src/instructions/lfork_cor.c \
			src/instructions/aff_cor.c \
			src/instructions/instruction.c \

NAME	= 	corewar

LIBS 	= 	my \

INCLUDE = 	include

CFLAGS = 	-I$(INCLUDE)

OBJ 	=	$(SRC:.c=.o)

LIBFLAGS	=	$(addprefix -l,$(LIBS))

all: libs_make $(OBJ) $(NAME)
	@echo "\e[1;31m\n - Project Built ! \n\e[0m"


$(NAME):
	gcc -I$(INCLUDE) -o $(NAME) $(OBJ) -Llib $(LIBFLAGS)

clean: libs_clean
	@$(foreach lib, $(LIBS), \
		rm -f ./lib/lib$(lib).a;)
	@rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

libs_make:
	@$(foreach lib, $(LIBS), \
		echo "" && \
		echo "\e[1;33mBuilding $(lib)...\e[0m" && \
		$(MAKE) -C ./lib/$(lib) && \
		echo "\e[1;33mlib$(lib) Built\e[0m" && \
		echo "";)

libs_clean:
	@echo "\e[1;31mCleaning libs...\e[0m"
	@$(foreach lib, $(LIBS), \
		$(MAKE) -C ./lib/$(lib) clean;)
	@echo "\e[1;32mLibs clean\e[0m"
