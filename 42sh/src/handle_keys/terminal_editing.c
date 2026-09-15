/*
** EPITECH PROJECT, 2025
** terminal_editing.c
** File description:
** terminal_editing.c
*/

#include "shell.h"

void setup_terminal(struct termios *old_term, struct termios *new_term)
{
    if (isatty(STDIN_FILENO)) {
        tcgetattr(STDIN_FILENO, old_term);
        *new_term = *old_term;
        new_term->c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, new_term);
    }
}

void restore_terminal(struct termios *old_term)
{
    if (isatty(STDIN_FILENO))
        tcsetattr(STDIN_FILENO, TCSANOW, old_term);
}
