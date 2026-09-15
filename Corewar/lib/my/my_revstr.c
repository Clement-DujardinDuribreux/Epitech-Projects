/*
** EPITECH PROJECT, 2024
** my_revstr
** File description:
** reverse a str
*/

#include "my.h"

char *my_revstr(char *str)
{
    char temp;
    int a = 0;
    int len = my_strlen(str);

    while (*str != '\0' && str < str - a + (len / 2)) {
        temp = str[0 + a];
        str[0 + a] = str[len - 1 - a];
        str[len - 1 - a] = temp;
        a++;
    }
    return str;
}
