/*
** EPITECH PROJECT, 2024
** sort.c
** File description:
** sort.c
*/

#include <stdlib.h>
#include "list.h"

static int len_linked_list(linked_list_t *begin)
{
    linked_list_t *node = begin;
    int len = 0;

    while (node != NULL) {
        len++;
        node = node->next;
    }
    return len;
}

static linked_list_t *get_box(linked_list_t *list, int n)
{
    linked_list_t *node = list;

    while (node != NULL && n != 0) {
        node = node->next;
        n--;
    }
    return node;
}

static void cut_linked_list(linked_list_t *list, int n)
{
    linked_list_t *node = list;

    while (node != NULL && n > 1) {
        node = node->next;
        n--;
    }
    node->next = NULL;
}

static linked_list_t *merge(linked_list_t *l1, linked_list_t *l2,
    int (*f)(void *, void *))
{
    if (l1 == NULL)
        return l2;
    if (l2 == NULL)
        return l1;
    if (!f(l1, l2)) {
        l1->next = merge(l1->next, l2, f);
        return l1;
    } else {
        l2->next = merge(l1, l2->next, f);
        return l2;
    }
}

linked_list_t *sort_merge(linked_list_t *list,
    int (*f)(void *, void *))
{
    int half_len = len_linked_list(list) / 2;
    linked_list_t *fhalf;
    linked_list_t *shalf;

    if (len_linked_list(list) <= 1)
        return list;
    else {
        fhalf = list;
        shalf = get_box(list, half_len);
        cut_linked_list(fhalf, half_len);
        return merge(sort_merge(fhalf, f),
            sort_merge(shalf, f), f);
    }
}
