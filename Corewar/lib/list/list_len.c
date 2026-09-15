/*
** EPITECH PROJECT, 2024
** list_len.c
** File description:
** list_len.c
*/

#include <stdlib.h>
#include "list.h"

int list_len(linked_list_t *list)
{
    int len = 0;

    while (list != NULL) {
        len++;
        list = list->next;
    }
    return len;
}
