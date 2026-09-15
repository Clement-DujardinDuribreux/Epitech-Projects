/*
** EPITECH PROJECT, 2025
** parentheses
** File description:
** parentheses
*/
#include "shell.h"

char *get_command_inside_parentheses(char *cmd)
{
    int before = 1;
    int after = my_strlen(cmd);
    char *new;
    int index = 0;

    for (int i = 0; cmd[i] != '\0' && cmd[i] != '('; i++)
        before++;
    for (int i = my_strlen(cmd); i >= 0 && cmd[i] != ')'; i--)
        after--;
    new = malloc(sizeof(char) * (after - before + 1));
    if (new == NULL)
        return NULL;
    for (int i = before; i < after; i++){
        new[index] = cmd[i];
        index++;
    }
    new[index] = '\0';
    return new;
}

int handle_parentheses(char *cmd_parentheses, shell_var_t *shell_var)
{
    pid_t pid = fork();
    int status;
    int return_value = 1;
    char *cmd;

    if (pid == -1)
        exit(84);
    if (pid == 0){
        cmd = get_command_inside_parentheses(cmd_parentheses);
        return_value = handle_several_commands(cmd, shell_var);
        free(cmd);
        exit(return_value);
    } else {
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
}
