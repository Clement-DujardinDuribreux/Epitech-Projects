##
## EPITECH PROJECT, 2024
## makefile
## File description:
## makefile
##

SRC     =   src/main.c \
			src/shell_prompt.c \
            src/parse/path.c \
            src/parse/globbing_handler.c \
            src/parse/globbing_utils.c \
            src/env/env.c \
            src/builtins/functions.c \
            src/env/variable.c \
            src/operations/pipe.c \
            src/operations/redirect.c \
            src/parse/handling_command.c \
            src/free/free_redirect.c \
			src/handle_keys/read_keys.c \
			src/handle_keys/terminal_editing.c \
			src/handle_keys/arrow_handling.c \
			src/handle_keys/line_edition.c \
			src/auto_comp/binary_utils.c \
			src/auto_comp/buffer_utils.c \
			src/auto_comp/completion_entry.c \
			src/auto_comp/directory_matcher.c \
			src/auto_comp/match_output.c \
			src/auto_comp/match_utils.c \
			src/auto_comp/path_utils.c \
			src/builtins/history.c \
			src/builtins/function_manage_env.c \
			src/builtins/local_var.c \
			src/mandatory/mandatory.c \
			src/mandatory/parentheses.c \
			src/builtins/aliases_var.c \
			src/execution/command_execution.c \
			src/builtins/special_var.c \

NAME	= 	42sh

LIBS 	= 	my \
			list \

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
