/*
** EPITECH PROJECT, 2024
** append.c
** File description:
** append.c
*/

#include "list.h"
#include <stdlib.h>

linked_list_t *append(linked_list_t *list, void *new_data)
{
    linked_list_t *new = malloc(sizeof(linked_list_t));

    new->data = new_data;
    new->next = list;
    return new;
}
