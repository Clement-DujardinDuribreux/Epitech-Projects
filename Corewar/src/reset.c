/*
** EPITECH PROJECT, 2025
** reset.c
** File description:
** reset.c
*/

#include "corewar.h"

void reset_args(char args[MAX_LENGHT_ARGS])
{
    for (int a = 0; a < MAX_LENGHT_ARGS; a++)
        args[a] = 0;
}

void reset_args_types(args_type_t types[MAX_ARGS_NUMBER])
{
    for (int a = 0; a < MAX_ARGS_NUMBER; a++)
        types[a] = 0;
}

void reset_reg(char reg[REG_NUMBER][REG_SIZE])
{
    for (int a = 0; a < REG_NUMBER; a++)
        for (int b = 0; b < REG_SIZE; b++)
            reg[a][b] = '\0';
}

void reset_map(char map[MEM_SIZE])
{
    for (int a = 0; a < MEM_SIZE; a++)
        map[a] = 0;
}
