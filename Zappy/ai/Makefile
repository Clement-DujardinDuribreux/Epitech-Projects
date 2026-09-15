##
## EPITECH PROJECT, 2026
## zappy
## File description:
## Makefile
##

ZAPPY_AI_SRC = main.py

zappy_ai: $(ZAPPY_AI_SRC)
	cp $(ZAPPY_AI_SRC) ../zappy_ai
	chmod +x ../zappy_ai

all:    zappy_ai

clean:
	find . -type d -name '__pycache__' -prune -exec rm -rf {} +
	rm -f ../zappy_ai

fclean: clean
	find . -type d -name '__pycache__' -prune -exec rm -rf {} +

re:     fclean all

.PHONY: all clean fclean re