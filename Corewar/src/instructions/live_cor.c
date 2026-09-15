/*
** EPITECH PROJECT, 2025
** live_cor.c
** File description:
** live instruction for Corewar
*/

#include "corewar.h"

void live_cor(robot_t *robot)
{
    robot->nb_bytes_args = 4;
}

void live_exe(robot_t *robot)
{
    robot_t *current = robot;

    while (current->parent != NULL)
        current = current->parent;
    current->live_id = get_int(robot->args, 4);
}
