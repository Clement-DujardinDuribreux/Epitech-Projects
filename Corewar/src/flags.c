/*
** EPITECH PROJECT, 2025
** main.c
** File description:
** main file for Corewar
*/

#include "corewar.h"

int nb_robots_init(robot_init_t **list)
{
    int result = 0;

    for (; list[result] != NULL; result++);
    return result;
}

int get_dump(int argc, char *argv[])
{
    for (int a = 1; a < argc; a++)
        if (!my_strcmp(argv[a], "-dump") &&
            argv[a + 1] != NULL && my_getnbr(argv[a + 1]) > 0)
                return my_getnbr(argv[a + 1]);
    return 0;
}

static int get_n_flag(int argc, char *argv[], int index)
{
    int i = index - 4 > 1 ? index - 4 : 1;

    for (; i < index; i++)
        if (!my_strcmp(argv[i], "-n") && my_getnbr(argv[i + 1]) > 0)
            return my_getnbr(argv[i + 1]);
    return 0;
}

static int get_a_flag(int argc, char *argv[], int index)
{
    int i = index - 4 > 1 ? index - 4 : 1;

    for (; i < index; i++)
        if (!my_strcmp(argv[i], "-a") && my_getnbr(argv[i + 1]) >= 0)
            return my_getnbr(argv[i + 1]);
    return -1;
}

static robot_init_t **set_init_champ(int argc, char *argv[],
    robot_init_t **list, int values[2])
{
    FILE *file = NULL;

    file = fopen(argv[values[1]], "r");
    list = realloc(list, sizeof(list) * sizeof(robot_init_t *));
    if (list == NULL)
        return NULL;
    if (file != NULL && list != NULL) {
        list[values[0]] = malloc(sizeof(robot_init_t));
        if (list[values[0]] == NULL)
            return NULL;
        list[values[0]]->file = file;
        list[values[0]]->id = get_n_flag(argc, argv, values[1]) != 0 ?
            get_n_flag(argc, argv, values[1]) : values[0] + 1;
        list[values[0]]->address = get_a_flag(argc, argv, values[1]) != -1 ?
            get_a_flag(argc, argv, values[1]) :
            (MEM_SIZE / (values[0] + 1)) * (values[0] > 0);
        list[values[0] + 1] = NULL;
    }
    return list;
}

robot_init_t **get_init_robot(int argc, char *argv[])
{
    robot_init_t **list = malloc(sizeof(robot_init_t *));
    int i = 0;

    if (list == NULL)
        return NULL;
    list[0] = NULL;
    for (int a = 1; a < argc; a++) {
        list = set_init_champ(argc, argv, list, (int[2]){i, a});
        if (list == NULL)
            return NULL;
        if (list[i] != NULL)
            i++;
    }
    return list;
}
