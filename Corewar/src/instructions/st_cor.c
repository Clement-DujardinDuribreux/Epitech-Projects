/*
** EPITECH PROJECT, 2025
** st_cor.c
** File description:
** st instruction for Corewar
*/

#include "corewar.h"

void st_cor(robot_t *robot)
{
    robot->nb_bytes_args = 1;
    for (int a = 0; a < MAX_ARGS_NUMBER; a++) {
        if (robot->args_type[a] == T_IND)
            robot->nb_bytes_args += 2;
        if (robot->args_type[a] == T_REG)
            robot->nb_bytes_args += 1;
    }
    return;
}

void st_exe(robot_t *robot)
{
    if (robot->args_type[1] == T_REG)
        convert_int_to_hex(get_int(robot->reg[robot->args[1] - 1], 4),
        robot->reg[robot->args[2]]);
    else
        convert_int_to_hex(get_int(robot->reg[robot->args[1] - 1], 4),
        &robot->area[robot->pos + get_int(&robot->args[2], 2) % IDX_MOD]);
    return;
}
