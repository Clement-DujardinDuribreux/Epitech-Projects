/*
** EPITECH PROJECT, 2024
** list.h
** File description:
** include lib/list
*/

#ifndef LIST
    #define LIST

typedef struct linked_list_s {
    void *data;
    struct linked_list_s *next;
} linked_list_t;

linked_list_t *add(linked_list_t *list, void *new_data);
linked_list_t *del(linked_list_t *list, int n,
    void (*free_node)(linked_list_t *));
void apply(linked_list_t *list, void (*f)(void *));
void apply_arg(linked_list_t *list, void (*f)(void *, void *), void *arg);
void *get_node(linked_list_t *list, int n);
int list_len(linked_list_t *list);
linked_list_t *sort_merge(linked_list_t *list,
    int (*f)(void *, void *));
linked_list_t *append_linked_list(linked_list_t *begin, void *data);

#endif /* LIST */
