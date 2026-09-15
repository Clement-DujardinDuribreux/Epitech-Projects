/*
** EPITECH PROJECT, 2024
** my_strstr
** File description:
** strstr function
*/

#include "my.h"
#include <stdlib.h>

int my_strstr(char *s1, char const *s2)
{
    int len = my_strlen(s2);

    if (s1 == NULL)
        return 0;
    for (int a = 0; s1[a] != '\0'; a++) {
        if (!my_strncmp(&s1[a], s2, len))
            return a;
    }
    return 0;
}
