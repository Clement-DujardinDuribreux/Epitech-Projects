/*
** EPITECH PROJECT, 2024
** my_strdup.c
** File description:
** allocate mem and copie the str given as param in it
*/
#include <stdlib.h>

char *my_strdup(char const *src)
{
    char *str;
    int taille_src = 0;
    int i = 0;

    if (src == NULL)
        return NULL;
    while (src[taille_src] != '\0')
        taille_src++;
    str = malloc(sizeof(char) * (taille_src + 1));
    while (i < taille_src){
        str[i] = src[i];
        i++;
    }
    str[i] = '\0';
    return str;
}
