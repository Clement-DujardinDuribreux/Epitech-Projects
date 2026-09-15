/*
** EPITECH PROJECT, 2025
** globbing_utils.c
** File description:
** globbing_utils.c
*/

#include "shell.h"

int has_glob_pattern(const char *str)
{
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '*' || str[i] == '?' || str[i] == '[' || str[i] == ']')
            return 1;
    }
    return 0;
}

int count_glob_matches(glob_t *globbuf)
{
    int count = 0;

    if (!globbuf)
        return 0;
    for (size_t i = 0; i < globbuf->gl_pathc; i++)
        count++;
    return count;
}

int calculate_expanded_size(char **args)
{
    int count = 0;

    for (int i = 0; args[i] != NULL; i++)
        count++;
    return count + 1;
}

char **init_expanded_args(int size)
{
    char **expanded_args = malloc(sizeof(char *) * size);

    if (!expanded_args)
        return NULL;
    return expanded_args;
}

char **realloc_expanded_args(char **expanded_args, int *expanded_size,
    int expanded_count, int needed_slots)
{
    int new_size = expanded_count + needed_slots + 1;
    char **temp = NULL;

    if (*expanded_size >= new_size)
        return expanded_args;
    temp = realloc(expanded_args, sizeof(char *) * new_size);
    if (!temp)
        return NULL;
    *expanded_size = new_size;
    return temp;
}
