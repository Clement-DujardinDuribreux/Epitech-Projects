/*
** EPITECH PROJECT, 2025
** ld_cor.c
** File description:
** ld instruction for Corewar
*/

#include "corewar.h"

void ld_cor(robot_t *robot)
{
    robot->nb_bytes_args = 1;
    for (int a = 0; a < MAX_ARGS_NUMBER; a++) {
        if (robot->args_type[a] == T_DIR)
            robot->nb_bytes_args += 4;
        if (robot->args_type[a] == T_IND)
            robot->nb_bytes_args += 2;
        if (robot->args_type[a] == T_REG)
            robot->nb_bytes_args += 1;
    }
    return;
}

void ld_exe(robot_t *robot)
{
    int load = 0;

    if (robot->args_type[0] == T_DIR)
        load = get_int(&robot->args[1], 4);
    if (robot->args_type[0] == T_IND)
        load = get_int(&robot->area[robot->pos +
            get_int(&robot->args[1], 2) % IDX_MOD], 4);
    convert_int_to_hex(load,
        robot->reg[robot->args[robot->nb_bytes_args - 1] - 1]);
    robot->carry = load != 0 ? 1 : 0;
    return;
}
