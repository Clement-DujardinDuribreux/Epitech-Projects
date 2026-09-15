/*
** EPITECH PROJECT, 2025
** add_cor.c
** File description:
** add instruction for Corewar
*/

#include "corewar.h"

void add_cor(robot_t *robot)
{
    robot->nb_bytes_args = 4;
}

void add_exe(robot_t *robot)
{
    char r1 = robot->args[1];
    char r2 = robot->args[2];
    char r3 = robot->args[3];
    int a = 0;

    a = get_int(robot->reg[r1 - 1], 4) + get_int(robot->reg[r2 - 1], 4);
    robot->carry = a != 0 ? 1 : 0;
    convert_int_to_hex(a, robot->reg[r3 - 1]);
}
