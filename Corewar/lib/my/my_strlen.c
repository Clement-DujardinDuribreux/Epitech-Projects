/*
** EPITECH PROJECT, 2024
** my_strlen.c
** File description:
** display the number of char
** in the string passed as parameter
*/
#include <unistd.h>

int my_strlen(char const *str)
{
    int i = 0;

    if (str == NULL)
        return 0;
    while (str[i] != '\0'){
        i++;
    }
    return i;
}
