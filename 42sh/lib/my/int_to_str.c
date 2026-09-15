/*
** EPITECH PROJECT, 2024
** int_to_str.c
** File description:
** int_to_str
*/

#include <stdlib.h>
#include "my.h"

static int unit(int nb)
{
    int result = 0;

    if (nb == 0)
        return 1;
    while (nb > 0) {
        nb -= nb % 10;
        nb /= 10;
        result++;
    }
    return result;
}

char *int_to_str(int nb)
{
    char *temp = malloc(sizeof(char) * unit(nb) + 1);
    int nbr = nb;

    if (temp == NULL)
        return NULL;
    for (int i = 0; i < unit(nb); i++){
        temp[i] = 48 + (nbr % 10);
        nbr -= nbr % 10;
        nbr /= 10;
    }
    temp[unit(nb)] = '\0';
    temp = my_revstr(temp);
    return temp;
}
