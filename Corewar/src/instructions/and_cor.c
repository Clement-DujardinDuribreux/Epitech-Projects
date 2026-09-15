/*
** EPITECH PROJECT, 2025
** and_cor.c
** File description:
** and instruction for Corewar
*/

#include "corewar.h"

void and_cor(robot_t *robot)
{
    robot->nb_bytes_args = 1;
    for (int i = 0; i < 3; i++) {
        if (robot->args_type[i] == T_DIR)
            robot->nb_bytes_args += 4;
        if (robot->args_type[i] == T_IND)
            robot->nb_bytes_args += 2;
        if (robot->args_type[i] == T_REG)
            robot->nb_bytes_args += 1;
    }
}

void and_exe(robot_t *robot)
{
    int value[2];
    int result = 0;
    int index = 0;

    for (int i = 0; i < 2; i++) {
        index += robot_reg(robot, i, value, index);
    }
    result = value[0] & value[1];
    robot->carry = result != 0 ? 1 : 0;
    convert_int_to_hex(result, &robot->args[index + 1]);
}
