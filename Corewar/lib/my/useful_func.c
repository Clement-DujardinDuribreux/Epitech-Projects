/*
** EPITECH PROJECT, 2024
** util_func
** File description:
** useful functions for my_printf
*/

#include <stdlib.h>
#include "my.h"

char *append(char *dest, char c)
{
    int a = 0;

    while (dest[a] != '\0')
        a++;
    dest[a] = c;
    dest[a + 1] = '\0';
    return dest;
}

void double_to_str(char *dest, double nb, int nb_decimal)
{
    int temp;
    int ent = nb;
    int a = 0;

    my_strcat(dest, int_to_str(ent));
    if (nb_decimal > 0)
        append(dest, '.');
    nb = (nb - ent) * 10;
    for (; a <= nb_decimal; a++) {
        temp = nb;
        if (a == nb_decimal && nb - temp >= 0.5)
            temp++;
        if (temp == 10) {
            dest[my_strlen(dest) - 1] += 1;
            temp = 0;
        }
        append(dest, temp + 48);
        nb = (nb - temp) * 10;
    }
    append(dest, '\0');
}

void round_char_double(char *str)
{
    for (int a = 0; str[a] != '\0'; a++)
        if (str[a] == ':') {
            str[a - 1] += 1;
            str[a] = '0';
            a -= 2;
        }
}

int is_letter(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return 1;
    return 0;
}
