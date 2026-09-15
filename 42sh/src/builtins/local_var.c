/*
** EPITECH PROJECT, 2025
** local_var.c.c
** File description:
** local variable handling
*/

#include "shell.h"

void my_str_realloc_cat(char **buff, variable_t *str, size_t *len)
{
    if (str == NULL || str->value == NULL)
        return;
    for (int a = 0; str->value[a] != '\0'; a++)
        add_char_to_buffer(buff, len, str->value[a], false);
}

static int get_n_node(linked_list_t *local, char *name)
{
    int n = 0;
    variable_t *node = get_node(local, n);

    if (node == NULL)
        return 0;
    do {
        if (!my_strcmp(name, node->name))
            return n;
        n++;
        node = get_node(local, n);
    } while (node != NULL);
    return n;
}

static char *get_name(char *buff)
{
    char *name = NULL;
    size_t len = 0;

    while (buff[len] != ' ' && buff[len] != '\0')
        add_char_to_buffer(&name, &len, buff[len], false);
    return name;
}

void add_char_to_buffer(char **buffer, size_t *buf_size, char c, bool local)
{
    if (isatty(STDIN_FILENO) && local)
        write(1, &c, 1);
    *buffer = realloc(*buffer, *buf_size + 2);
    (*buffer)[*buf_size] = c;
    (*buf_size)++;
    (*buffer)[*buf_size] = '\0';
}

static void replace(char *buff, char **nbuff,
    linked_list_t *vars, bool replace_null)
{
    size_t len = 0;
    char *name = NULL;
    variable_t *temp = NULL;

    for (int i = 0; buff[i] != '\0'; i++) {
        if (buff[i] == '$') {
            name = get_name(&buff[i + 1]);
            temp = get_node(vars, get_n_node(vars, name));
            my_str_realloc_cat(nbuff, temp, &len);
        } else
            add_char_to_buffer(nbuff, &len, buff[i], false);
        if (buff[i] == '$' && temp == NULL && !replace_null)
            add_char_to_buffer(nbuff, &len, buff[i], false);
        i += buff[i] == '$' && name != NULL && (temp != NULL || replace_null)
            ? strlen(name) : 0;
        if (name != NULL)
            free(name);
        name = NULL;
    }
}

char *replace_var(char *buff, linked_list_t *vars, bool replace_null)
{
    char *nbuff = NULL;

    if (!my_strstr(buff, "$"))
        return buff;
    replace(buff, &nbuff, vars, replace_null);
    free(buff);
    return nbuff;
}
