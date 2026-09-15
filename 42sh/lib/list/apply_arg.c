/*
** EPITECH PROJECT, 2024
** disp.c
** File description:
** disp.c
*/

#include "list.h"
#include <stdlib.h>

void apply_arg(linked_list_t *list, void (*f)(void *, void *), void *arg)
{
    while (list != NULL) {
        if (list->data != NULL)
            f(list->data, arg);
        list = list->next;
    }
}
