/*
** EPITECH PROJECT, 2025
** free_redirect
** File description:
** free_redirect
*/
#include "shell.h"

void free_redirect(redirection_t *redirection)
{
    free_word_array(redirection->redirected);
    free_word_array(redirection->redirections);
    free(redirection);
}
