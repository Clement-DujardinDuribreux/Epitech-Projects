/*
** EPITECH PROJECT, 2025
** zjmp_cor.c
** File description:
** zjmp instruction for Corewar
*/

#include "corewar.h"

void zjmp_cor(robot_t *robot)
{
    robot->nb_bytes_args = 2;
}

void zjmp_exe(robot_t *robot)
{
    robot->pos += get_int(robot->args, 2) - robot->nb_bytes_args - 1;
    return;
}
