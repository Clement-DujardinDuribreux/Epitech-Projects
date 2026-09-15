/*
** EPITECH PROJECT, 2025
** intruction.c
** File description:
** instruction.c
*/

#include "corewar.h"

void convert_int_to_hex(int dec, char hex[REG_SIZE])
{
    int i = 0;

    while (i < REG_SIZE) {
        hex[REG_SIZE - 1 - i] = dec % 256;
        i++;
        dec /= 256;
    }
}

void cpy_robot(robot_t *src, robot_t *dest)
{
    dest->area = src->area;
    dest->childs = NULL;
    dest->id = src->id;
    dest->is_alive = 1;
    dest->count = 0;
    my_strcpy(dest->name, src->name);
    my_strcat(dest->name, " child");
    reset_args(dest->args);
    reset_args_types(dest->args_type);
    reset_reg(dest->reg);
}

int robot_reg(robot_t *robot, int i, int value[2], int index)
{
    if (robot->args_type[i] == T_DIR) {
        value[i] = get_int(&robot->args[index + 1], 4);
        return 4;
    }
    if (robot->args_type[i] == T_IND) {
        value[i] = get_int(&robot->area[robot->pos +
            get_int(&robot->args[index + 1], 4) % IDX_MOD], 4);
        return 2;
    }
    if (robot->args_type[i] == T_REG) {
        value[i] = get_int(robot->reg[robot->args[index + 1]], 4);
        return 1;
    }
    return 0;
}

char *decimal_to_base(char *base, unsigned char nb)
{
    int len_base = my_strlen(base);
    char *result;
    int verif = nb / len_base;
    int a = 0;

    result = malloc(sizeof(char) * 3);
    while (nb > len_base) {
        result[a] = base[nb % len_base];
        nb = verif;
        verif = nb / len_base;
        a++;
    }
    result[a] = base[nb % len_base];
    a++;
    for (; a < 2; a++)
        result[a] = '0';
    result[a] = '\0';
    my_revstr(result);
    return result;
}
