/*
** EPITECH PROJECT, 2025
** arrow_handling.c
** File description:
** arrow_handling.c
*/

#include "shell.h"

static void set_id(int *id, int arrow, int max)
{
    if (*id == 0)
        *id = max + 1;
    if (arrow == UP)
        *id = *id > 1 ? *id - 1 : 1;
    else if (arrow == DOWN)
        *id = *id < max ? *id + 1 : max + 1;
}

static char *process_history_arrow(int *id, char *buff, int arrow,
    linked_list_t *env)
{
    char *temp = NULL;
    char *path = get_history_path(env);
    int max = 0;

    if (path == NULL) {
        *id = 0;
        return NULL;
    }
    max = get_id(path);
    free(path);
    set_id(id, arrow, max);
    if (buff != NULL) {
        supp_buff(buff);
        free(buff);
    }
    temp = call_history(*id, env);
    if (temp != NULL)
        write(1, temp, strlen(temp));
    return temp;
}

void handling_arrows(char **buff, int arrow, int **editing,
    linked_list_t **env)
{
    if (arrow == UP || arrow == DOWN) {
        reset_cursor(*buff, editing[1]);
        *buff = process_history_arrow(editing[0], *buff, arrow, *env);
        if (*buff == NULL)
            *editing[1] = 0;
        else
            *editing[1] = strlen(*buff);
    }
    if (arrow == LEFT || arrow == RIGHT)
        move_cursor(arrow, editing[1], *buff);
}
