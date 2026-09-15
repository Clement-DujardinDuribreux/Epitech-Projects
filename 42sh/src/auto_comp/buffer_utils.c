/*
** EPITECH PROJECT, 2025
** buffer_utils.c
** File description:
** buffer_utils.c
*/

#include "shell.h"

int get_last_space_position(char *buffer)
{
    int i = 0;
    int last_space = -1;

    if (buffer == NULL)
        return -1;
    while (buffer[i] != '\0') {
        if (buffer[i] == ' ')
            last_space = i;
        i++;
    }
    return last_space;
}

char *get_partial_path(char *buffer)
{
    int last_space = 0;

    if (buffer == NULL)
        return NULL;
    last_space = get_last_space_position(buffer);
    if (last_space == -1)
        return buffer;
    return buffer + last_space + 1;
}

void complete_exact_match(char **buffer, char *exact_match, char *prefix)
{
    char *new_buffer = NULL;
    int prefix_len = 0;
    int completion_len = 0;
    int buffer_len = 0;

    prefix_len = my_strlen(prefix);
    completion_len = my_strlen(exact_match) - prefix_len;
    if (completion_len <= 0)
        return;
    buffer_len = my_strlen(*buffer);
    new_buffer = malloc(sizeof(char) * (buffer_len + completion_len + 1));
    if (new_buffer == NULL)
        return;
    my_strncpy(new_buffer, *buffer, buffer_len);
    my_strncpy(new_buffer + buffer_len,
        exact_match + prefix_len, completion_len);
    new_buffer[buffer_len + completion_len] = '\0';
    free(*buffer);
    *buffer = new_buffer;
}

void swap_strings(char **str1, char **str2)
{
    char *temp = NULL;

    if (strcmp(*str1, *str2) > 0) {
        temp = *str1;
        *str1 = *str2;
        *str2 = temp;
    }
}

void sort_matches(char **matches, int match_count)
{
    for (int i = 0; i < match_count - 1; i++) {
        for (int j = i + 1; j < match_count; j++)
            swap_strings(&matches[i], &matches[j]);
    }
}
