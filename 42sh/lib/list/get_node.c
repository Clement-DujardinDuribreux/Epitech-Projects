/*
** EPITECH PROJECT, 2024
** get_node.c
** File description:
** get_node.c
*/

#include <stdlib.h>
#include "list.h"

void *get_node(linked_list_t *list, int n)
{
    for (; list != NULL && n > 0; n--)
        list = list->next;
    if (list == NULL)
        return NULL;
    return list->data;
}
