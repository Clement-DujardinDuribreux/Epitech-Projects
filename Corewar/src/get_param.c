/*
** EPITECH PROJECT, 2025
** get_param
** File description:
** get_param
*/
#include "corewar.h"

void get_args(char args[MAX_LENGHT_ARGS], char *area, int pos, int nb)
{
    for (int a = 0; a < nb; a++)
        args[a] = area[pos + a];
}

char get_type(char binary)
{
    if (binary == 1)
        return T_REG;
    if (binary == 2)
        return T_DIR;
    if (binary == 3)
        return T_IND;
    return 0;
}

void get_args_types(args_type_t args_types[MAX_ARGS_NUMBER],
    unsigned char coding_byte)
{
    char temp = 0;
    int i = 0;

    temp = coding_byte;
    while (i < MAX_ARGS_NUMBER) {
        args_types[MAX_ARGS_NUMBER - 1 - i] = get_type(temp & 0b11);
        i++;
        temp = temp >> 2;
    }
}

int get_int(char *hexa, int nb_bytes)
{
    int result = 0;

    for (int i = 0; i < nb_bytes; i++) {
        result = (result << 8) | hexa[i];
    }
    return result;
}
