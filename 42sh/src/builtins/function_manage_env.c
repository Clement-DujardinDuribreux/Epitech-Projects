/*
** EPITECH PROJECT, 2025
** function_manage_env.c
** File description:
** function_manage_env
*/

#include "shell.h"

static void handling_first_current(linked_list_t **begin, void *ref)
{
    linked_list_t *current;

    current = *begin;
    while (current != NULL){
        if (!my_strcmp((char *){ref}, (variable_t *){current->data}->name)){
            *begin = current->next;
            free((variable_t *){current->data}->name);
            free((variable_t *){current->data}->value);
            free(current->data);
            free(current);
            current = *begin;
        } else
            break;
    }
}

static void delete_in_list(linked_list_t **begin, void *ref)
{
    linked_list_t *current;
    linked_list_t *supp;

    handling_first_current(begin, ref);
    current = *begin;
    while (current != NULL){
        if (current->next == NULL)
            break;
        if (!my_strcmp((char *){ref},
            (variable_t *){current->next->data}->name)){
            supp = current->next;
            current->next = supp->next;
            free((variable_t *){supp->data}->name);
            free((variable_t *){supp->data}->value);
            free(supp->data);
            free(supp);
        } else
            current = current->next;
    }
}

int setenv_shell(char *line, shell_var_t *shell_var)
{
    char **array = my_str_to_word_array(line, " \t");

    if (len_word_array(array) >= 2){
        if (is_alphanumeric(array[1]))
            set_env(array[1], array[2], &shell_var->env);
        else {
            write(1, "setenv: Variable name must contain "
                "alphanumeric characters.\n", 60);
            return 1;
        }
        free_word_array(array);
        return 0;
    }
    free_word_array(array);
    return 1;
}

int unsetenv_shell(char *line, shell_var_t *shell_var)
{
    char **word_array = my_str_to_word_array(line, " \t");

    if (len_word_array(word_array) == 2){
        delete_in_list(&shell_var->env, word_array[1]);
        free_word_array(word_array);
        return 0;
    }
    free_word_array(word_array);
    return 1;
}

int env_shell(char *line, shell_var_t *shell_var)
{
    linked_list_t *var_env = shell_var->env;
    shell_var_t shell = {.env = var_env->next, .local = shell_var->local};

    if (var_env != NULL){
        if (var_env->next != NULL)
            env_shell(line, &shell);
        write(1, (variable_t *){var_env->data}->name,
            my_strlen((variable_t *){var_env->data}->name));
        write(1, "=", 1);
        write(1, (variable_t *){var_env->data}->value,
            my_strlen((variable_t *){var_env->data}->value));
        write(1, "\n", 1);
        return 0;
    }
    return 1;
}
