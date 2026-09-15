/*
** EPITECH PROJECT, 2024
** my_put_nbr
** File description:
** print a number
*/

#include <stdlib.h>
#include <unistd.h>
#include "my.h"

int my_put_nbr(int nb)
{
    char *nb_str = int_to_str(nb);

    write(1, nb_str, sizeof(nb_str));
    free(nb_str);
    return 0;
}
