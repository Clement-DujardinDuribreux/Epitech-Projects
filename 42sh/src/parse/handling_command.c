/*
** EPITECH PROJECT, 2025
** handling_command
** File description:
** handling_command
*/
#include "shell.h"

char *command(char *cmd, linked_list_t *env)
{
    if (cmd == NULL)
        return NULL;
    for (int i = 0; cmd[i] != '\0'; i++)
        if (cmd[i] == '/')
            return my_strdup(cmd);
    return pathing(cmd, env);
}

static int check_command_exist(char *command_value, char *command)
{
    if (command_value != NULL && !access(command_value, F_OK))
        return 1;
    write(1, command, my_strlen(command));
    write(1, ": Command not found.\n", 21);
    return 0;
}

static int check_command_permission(char *command_value, char *command)
{
    if (!access(command_value, X_OK))
        return 1;
    write(1, command, my_strlen(command));
    write(1, ": Permission denied.\n", 21);
    return 0;
}

int handle_command(char *command_value, char *command)
{
    if (check_command_exist(command_value, command))
        if (check_command_permission(command_value, command))
            return 1;
    return 0;
}

int is_alphanumeric(char *str)
{
    for (int i = 0; str[i] != '\0'; i++){
        if ((str[i] < 48 || str[i] > 57) &&
            (str[i] < 65 || str[i] > 90) &&
            (str[i] < 97 || str[i] > 122))
            return 0;
    }
    return 1;
}
