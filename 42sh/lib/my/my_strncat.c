/*
** EPITECH PROJECT, 2024
** my_strncat
** File description:
** concatenate n characters of src
*/

#include "my.h"

char *my_strncat(char *dest, char const *src, int nb)
{
    int len = my_strlen(dest);
    int b = 0;

    while (src[b] != '\0' && b < nb) {
        dest[len + b] = src[b];
        b++;
    }
    dest[len + b] = '\0';
    return dest;
}
