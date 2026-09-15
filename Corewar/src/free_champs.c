/*
** EPITECH PROJECT, 2025
** free_champs
** File description:
** free_champs
*/

#include "corewar.h"

void free_champs(robot_t **champs)
{
    for (int i = 0; champs != NULL && champs[i] != NULL; i++) {
        free_champs(champs[i]->childs);
        free(champs[i]);
    }
    free(champs);
}

void free_robot_init(robot_init_t **list)
{
    for (int i = 0; list != NULL && list[i] != NULL; i++) {
        fclose(list[i]->file);
        free(list[i]);
    }
    free(list);
}
