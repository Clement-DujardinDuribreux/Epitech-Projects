/*
** EPITECH PROJECT, 2025
** ldi_cor.c
** File description:
** ldi instruction for Corewar
*/

#include "corewar.h"

void ldi_cor(robot_t *robot)
{
    robot->nb_bytes_args = 1;
    for (int a = 0; a < MAX_ARGS_NUMBER; a++) {
        if (robot->args_type[a] == T_DIR)
            robot->nb_bytes_args += 2;
        if (robot->args_type[a] == T_IND)
            robot->nb_bytes_args += 2;
        if (robot->args_type[a] == T_REG)
            robot->nb_bytes_args += 1;
    }
    return;
}

void ldi_exe(robot_t *robot)
{
    int i[3] = {0, 0, 0};

    if (robot->args_type[0] == T_IND)
        i[0] = get_int(&robot->area[robot->pos +
            get_int(&robot->args[1], 2) % IDX_MOD], 2);
    if (robot->args_type[0] == T_DIR)
        i[0] = get_int(&robot->args[1], 2);
    if (robot->args_type[0] == T_REG)
        i[0] = get_int(robot->reg[robot->args[1] - 1], 4);
    i[2] = 2 + (robot->args_type[0] == T_IND || robot->args_type[0] == T_DIR);
    if (robot->args_type[1] == T_DIR)
        i[1] = get_int(&robot->args[i[2]], 2);
    if (robot->args_type[1] == T_REG)
        i[1] = get_int(robot->reg[robot->args[i[2]] - 1], 4);
    i[2] += 1 + (robot->args_type[1] == T_DIR);
    convert_int_to_hex(get_int(&robot->area[robot->pos +
        (i[0] + i[1]) % IDX_MOD], 4), robot->reg[robot->args[i[2]] - 1]);
    robot->carry = get_int(&robot->area[robot->pos +
        (i[0] + i[1]) % IDX_MOD], 4) != 0 ? 1 : 0;
    return;
}
