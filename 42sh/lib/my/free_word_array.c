/*
** EPITECH PROJECT, 2024
** my.h
** File description:
** proto of all functions
*/

#include <stdlib.h>

void free_word_array(char **word_array)
{
    for (int a = 0; word_array[a] != NULL; a++)
        free(word_array[a]);
    free(word_array);
}
