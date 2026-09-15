/*
** EPITECH PROJECT, 2025
** append_linked_list.c
** File description:
** append for linked_list
*/

#include <stdlib.h>
#include "list.h"

linked_list_t *append_linked_list(linked_list_t *begin, void *data)
{
    linked_list_t *new_node = malloc(sizeof(linked_list_t));
    linked_list_t *temp = begin;

    new_node->next = NULL;
    new_node->data = data;
    if (begin == NULL)
        return new_node;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = new_node;
    return begin;
}
