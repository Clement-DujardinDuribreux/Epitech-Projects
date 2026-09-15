/*
** EPITECH PROJECT, 2025
** env.c
** File description:
** env.c
*/
#include "shell.h"

void push_to_list(linked_list_t **begin, void *data)
{
    linked_list_t *new_linked_list = malloc(sizeof(*new_linked_list));

    if (new_linked_list != NULL)
        new_linked_list->data = data;
        new_linked_list->next = *begin;
        *begin = new_linked_list;
}

linked_list_t *set_env_linked_list(char **env)
{
    linked_list_t *list = NULL;
    char **array;

    for (int i = 0; env[i] != NULL; i++){
        array = manage_env_separator(env[i], '=');
        push_to_list(&list, set_variable(array[0], array[1]));
        free_word_array(array);
    }
    push_to_list(&list, set_variable(
        "NLSPATH",
        "/usr/share/locale/%L/LC_MESSAGES/%N.cat"
        ":/usr/share/locale/%l/LC_MESSAGES/%N.cat"));
    return list;
}

char *transform_data(variable_t *data)
{
    char *variable = malloc(sizeof(char) *
        (my_strlen(data->name) + my_strlen(data->value) + 2));

    if (variable == NULL)
        return NULL;
    variable = my_strcpy(variable, data->name);
    variable = my_strcat(variable, "=");
    if (data->value != NULL)
        variable = my_strcat(variable, data->value);
    return variable;
}

static int get_len_list(linked_list_t *list)
{
    linked_list_t *current = list;
    int len = 0;

    while (current != NULL){
        len++;
        current = current->next;
    }
    return len;
}

char **linked_list_to_word_array(linked_list_t *list)
{
    linked_list_t *current = list;
    int len = get_len_list(list);
    char **array = malloc(sizeof(char *) * (len + 1));
    int index = 0;

    if (array == NULL)
        return NULL;
    while (current != NULL){
        array[index] = transform_data((variable_t *){current->data});
        index++;
        current = current->next;
    }
    array[index] = NULL;
    return array;
}

void free_list(linked_list_t *list)
{
    if (list->next != NULL)
        free_list(list->next);
    if ((variable_t *){list->data}->name != NULL)
        free((variable_t *){list->data}->name);
    if ((variable_t *){list->data}->value != NULL)
        free((variable_t *){list->data}->value);
    if (list->data != NULL)
        free(list->data);
    free(list);
    list = NULL;
}
