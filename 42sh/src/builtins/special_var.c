/*
** EPITECH PROJECT, 2025
** special_var.c
** File description:
** special variable handling for 42sh
*/

#include "shell.h"

static linked_list_t *basics_special(linked_list_t *special,
    int argc, char **argv)
{
    variable_t *temp = NULL;

    temp = malloc(sizeof(variable_t));
    temp->name = my_strdup("#");
    temp->value = int_to_str(argc);
    special = add(special, temp);
    temp = malloc(sizeof(variable_t));
    temp->name = my_strdup("$");
    temp->value = int_to_str(getpid());
    special = add(special, temp);
    temp = malloc(sizeof(variable_t));
    temp->name = my_strdup("term");
    temp->value = my_strdup("xterm-256color");
    special = add(special, temp);
    temp = malloc(sizeof(variable_t));
    temp->name = my_strdup("cwd");
    temp->value = getcwd(NULL, 255);
    special = add(special, temp);
    return special;
}

linked_list_t *get_specials_var(int argc, char **argv)
{
    linked_list_t *special = NULL;
    variable_t *temp = NULL;

    temp = malloc(sizeof(variable_t));
    temp->name = my_strdup("?");
    temp->value = int_to_str(0);
    special = add(special, temp);
    special = basics_special(special, argc, argv);
    for (int a = 0; a < argc; a++) {
        temp = malloc(sizeof(variable_t));
        temp->name = int_to_str(a);
        temp->value = strdup(argv[a]);
        special = add(special, temp);
    }
    return special;
}
