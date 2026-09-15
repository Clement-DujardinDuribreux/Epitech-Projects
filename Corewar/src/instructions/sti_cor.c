/*
** EPITECH PROJECT, 2025
** sti_cor.c
** File description:
** sti instruction for Corewar
*/

#include "corewar.h"

void sti_cor(robot_t *robot)
{
    robot->nb_bytes_args = 1;
    for (int a = 0; a < MAX_ARGS_NUMBER; a++) {
        if (robot->args_type[a] == T_DIR || robot->args_type[a] == T_IND)
            robot->nb_bytes_args += 2;
        if (robot->args_type[a] == T_REG)
            robot->nb_bytes_args += 1;
    }
}

void sti_exe(robot_t *robot)
{
    int value1 = 0;
    int value2 = 0;
    int reg_value = 0;
    int i = 0;

    reg_value = get_int(robot->reg[robot->args[1] - 1], 4);
    if (robot->args_type[1] == T_REG)
        value1 = get_int(robot->reg[robot->args[2] - 1], 4);
    if (robot->args_type[1] == T_DIR)
        value1 = get_int(&robot->args[2], 2);
    if (robot->args_type[1] == T_IND)
        value1 = get_int(&robot->area[get_int(&robot->args[2], 2)], 4);
    i = 3 + (robot->args_type[1] == T_DIR || robot->args_type[1] == T_IND);
    if (robot->args_type[2] == T_DIR)
        value2 = get_int(&robot->args[i], 2);
    if (robot->args_type[2] == T_REG)
        value2 = get_int(robot->reg[robot->args[i] - 1], 4);
    convert_int_to_hex(reg_value, &robot->area[robot->pos +
        (value1 + value2) % IDX_MOD]);
    return;
}
