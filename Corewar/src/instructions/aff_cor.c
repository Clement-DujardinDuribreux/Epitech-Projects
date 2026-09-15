/*
** EPITECH PROJECT, 2025
** aff_cor.c
** File description:
** aff instruction for Corewar
*/

#include "corewar.h"

void aff_cor(robot_t *robot)
{
    robot->nb_bytes_args = 2;
}

void aff_exe(robot_t *robot)
{
    write(1, robot->reg[robot->args[1] - 1], REG_SIZE);
}
