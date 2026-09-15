/*
** EPITECH PROJECT, 2025
** for_cor.c
** File description:
** fork instruction for Corewar
*/

#include "corewar.h"

void fork_cor(robot_t *robot)
{
    robot->nb_bytes_args = 2;
    return;
}

void fork_exe(robot_t *robot)
{
    int a = 0;

    if (!robot->carry)
        return;
    for (; robot->childs != NULL && robot->childs[a] != NULL; a++);
    robot->childs = realloc(robot->childs, sizeof(robot_t *) * (a + 2));
    robot->childs[a] = malloc(sizeof(robot_t));
    if (robot->childs[a] == NULL)
        return;
    robot->childs[a + 1] = NULL;
    robot->childs[a]->pos = robot->pos + get_int(robot->args, 2) % IDX_MOD;
    cpy_robot(robot, robot->childs[a]);
    robot->childs[a]->code_instruction = robot->area[robot->childs[a]->pos];
    robot->childs[a]->need =
        op_tab[robot->childs[a]->code_instruction].nbr_cycles;
    robot->childs[a]->parent = robot;
    robot->childs[a]->carry = 1;
    robot->childs[a]->live_id = 0;
    robot->childs[a]->alive_count = 0;
    return;
}
