/*
** EPITECH PROJECT, 2025
** aliases_var.c
** File description:
** aliases_var
*/
#include "shell.h"

static char *replace_alias(char *alias, char **args)
{
    char *nline = NULL;

    my_str_realloc_cat(&nline, &(variable_t){.value = alias},
    &(size_t){0});
    for (int a = 0; nline != NULL && args != NULL && args[a] != NULL; a++) {
        my_str_realloc_cat(&nline, &(variable_t){.value = " "},
        &(size_t){strlen(nline)});
        my_str_realloc_cat(&nline, &(variable_t){.value = args[a]},
        &(size_t){strlen(nline)});
    }
    return nline;
}

char *get_buffer_alias(char *buff, linked_list_t *aliases)
{
    variable_t *alias = NULL;
    char **word_array = NULL;

    if (buff == NULL)
        return NULL;
    word_array = my_str_to_word_array(buff, " \t");
    if (word_array == NULL)
        return NULL;
    while (aliases != NULL) {
        alias = aliases->data;
        if (alias != NULL && !my_strcmp(word_array[0], alias->name)
            && alias->value != NULL) {
                buff = replace_alias(alias->value, &word_array[1]);
                free_word_array(word_array);
                return buff;
        }
        aliases = aliases->next;
    }
    free_word_array(word_array);
    return buff;
}

int alias_shell(char *line, shell_var_t *shell_var)
{
    char **word_array = my_str_to_word_array(line, " \t");
    variable_t *data = NULL;

    if (word_array == NULL)
        return 1;
    if (len_word_array(word_array) != 3) {
        free_word_array(word_array);
        return 1;
    }
    data = malloc(sizeof(variable_t));
    if (data == NULL) {
        free_word_array(word_array);
        return 1;
    }
    data->name = my_strdup(word_array[1]);
    data->value = my_strdup(word_array[2]);
    shell_var->aliases = add(shell_var->aliases, data);
    free_word_array(word_array);
    return 0;
}

linked_list_t *get_base_aliases(void)
{
    linked_list_t *aliases = NULL;
    variable_t *temp = NULL;

    temp = malloc(sizeof(variable_t));
    temp->name = my_strdup("ls");
    temp->value = my_strdup("ls --color");
    aliases = add(aliases, temp);
    temp = malloc(sizeof(variable_t));
    temp->name = my_strdup("l");
    temp->value = my_strdup("ls -l --color -a");
    aliases = add(aliases, temp);
    temp = malloc(sizeof(variable_t));
    temp->name = my_strdup("ll");
    temp->value = my_strdup("ls -l --color");
    aliases = add(aliases, temp);
    return aliases;
}
