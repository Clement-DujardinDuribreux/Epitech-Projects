/*
** EPITECH PROJECT, 2025
** in_word_array.c
** File description:
** in_word_array
*/

#include <stdlib.h>
#include "my.h"

int in_word_array(char **word_array, char *find)
{
    for (int a = 0; word_array[a] != NULL; a++)
        if (!my_strcmp(word_array[a], find))
            return a;
    return -1;
}
