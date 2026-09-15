/*
** EPITECH PROJECT, 2024
** del.c
** File description:
** del.c
*/

#include <stdlib.h>
#include "list.h"

linked_list_t *del(linked_list_t *list, int n,
    void (*free_node)(linked_list_t *))
{
    linked_list_t *temp = NULL;
    linked_list_t *begin = list;

    if (n == 0) {
        list = list->next;
        free_node(begin);
        return list;
    }
    for (int i = 0; list != NULL && list->next != NULL && i < n - 1; i++)
        list = list->next;
    temp = list->next;
    list->next = list->next->next;
    free_node(temp);
    return begin;
}
