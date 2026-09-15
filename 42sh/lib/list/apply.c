/*
** EPITECH PROJECT, 2024
** disp.c
** File description:
** disp.c
*/

#include "list.h"
#include <stdlib.h>

void apply(linked_list_t *list, void (*f)(void *))
{
    while (list != NULL) {
        if (list->data != NULL)
            f(list->data);
        list = list->next;
    }
}
