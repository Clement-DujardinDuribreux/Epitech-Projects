/*
** EPITECH PROJECT, 2025
** command_execution.c
** File description:
** Functions for executing commands
*/
#include "shell.h"

static char **prepare_arguments(char *line)
{
    char **word_array = my_str_to_word_array(line, " \t");
    char **expanded_args;

    if (!word_array || !word_array[0]) {
        if (word_array)
            free_word_array(word_array);
        return NULL;
    }
    expanded_args = expand_glob_patterns(word_array);
    free_word_array(word_array);
    if (!expanded_args || !expanded_args[0]) {
        if (expanded_args)
            free_word_array(expanded_args);
        return NULL;
    }
    return expanded_args;
}

static char **separe_command_line(char *line)
{
    char **word_array = my_str_to_word_array(line, " \t");

    if (line == NULL)
        return NULL;
    if (!word_array || !word_array[0]) {
        if (word_array)
            free_word_array(word_array);
        return NULL;
    }
    return word_array;
}

int is_parentheses_command(const char *line)
{
    char **word_array = my_str_to_word_array(line, " \t");

    if (word_array[0][0] == '(' &&
        word_array[len_word_array(word_array) - 1]
        [strlen(word_array[len_word_array(word_array) - 1]) - 1]){
        free_word_array(word_array);
        return 1;
    } else {
        free_word_array(word_array);
        return 0;
    }
}

char **process_args(char **word_array)
{
    char **expanded_args = expand_glob_patterns(word_array);

    free_word_array(word_array);
    if (!expanded_args || !expanded_args[0]) {
        if (expanded_args)
            free_word_array(expanded_args);
        return NULL;
    }
    return expanded_args;
}

static int prepare_and_execute(char *cmd_name, char **args,
    shell_var_t *shell_var, redirection_t *redirection)
{
    char **env_array;
    int return_value;

    env_array = linked_list_to_word_array(shell_var->env);
    return_value = switching_process(cmd_name, args, env_array, redirection);
    free(cmd_name);
    free_word_array(env_array);
    free_word_array(args);
    return return_value;
}

int execute_process(char *line, shell_var_t *shell_var,
    redirection_t *redirection)
{
    char **word_array;
    char **expanded_args;
    char *cmd;
    int return_value = 0;

    word_array = separe_command_line(line);
    if (!word_array)
        return 0;
    if (is_parentheses_command(line)) {
        return_value = handle_parentheses(line, shell_var);
        free_word_array(word_array);
        return return_value;
    }
    expanded_args = process_args(word_array);
    if (!expanded_args)
        return 0;
    cmd = command(*expanded_args, shell_var->env);
    return_value = prepare_and_execute(cmd, expanded_args,
        shell_var, redirection);
    return return_value;
}
