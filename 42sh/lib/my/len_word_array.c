/*
** EPITECH PROJECT, 2025
** len_word_array.c
** File description:
** len_word_array
*/

#include <stdlib.h>

int len_word_array(char **word_array)
{
    int a = 0;

    for (; word_array[a] != NULL; a++);
    return a;
}
