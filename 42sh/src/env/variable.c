/*
** EPITECH PROJECT, 2025
** variable.c
** File description:
** variable.c
*/
#include "shell.h"

variable_t *set_variable(char *name, char *value)
{
    variable_t *variable = malloc(sizeof(*variable));

    if (variable == NULL)
        return NULL;
    variable->name = my_strdup(name);
    if (value != NULL)
        variable->value = my_strdup(value);
    else
        variable->value = NULL;
    return variable;
}

static char *get_value(char **values)
{
    char *value;
    int len_value = 0;

    for (int i = 0; values[i] != NULL; i++)
        len_value += my_strlen(values[i]);
    if (len_value == 0)
        return NULL;
    value = malloc(sizeof(char) * (len_value + len_word_array(values)));
    if (value == NULL)
        return NULL;
    value = my_strcpy(value, values[0]);
    for (int i = 1; values[i] != NULL; i++){
        value = my_strcat(value, "=");
        value = my_strcat(value, values[i]);
    }
    return value;
}

char **manage_env_separator(char *str, char separator)
{
    int i = 0;
    char **word_array;
    char **temp = my_str_to_word_array(str, "=");

    for (; str[i] != '\0' && str[i] != separator; i++);
    if (i == my_strlen(str) || temp == NULL){
        free_word_array(temp);
        return NULL;
    }
    word_array = malloc(sizeof(char *) * 3);
    if (word_array == NULL){
        free_word_array(temp);
        return NULL;
    }
    word_array[0] = my_strdup(*temp);
    word_array[1] = get_value(temp + 1);
    word_array[2] = NULL;
    free_word_array(temp);
    return word_array;
}
