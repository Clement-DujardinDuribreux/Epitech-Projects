/*
** EPITECH PROJECT, 2025
** path
** File description:
** path
*/
#include "shell.h"

linked_list_t *check_env_variable(linked_list_t *env, char *variable)
{
    linked_list_t *current = env;

    for (; current != NULL; current = current->next){
        if (!my_strcmp(variable, (variable_t *){current->data}->name))
            return current;
    }
    return NULL;
}

char *concat_pathing(char *file, char *current)
{
    char *path;

    path = malloc(my_strlen(current) + my_strlen(file) + 2);
    if (path == NULL)
        return NULL;
    path = my_strcpy(path, current);
    path = my_strcat(path, "/");
    path = my_strcat(path, file);
    return path;
}

char *pathing(char *cmd, linked_list_t *env)
{
    linked_list_t *index = check_env_variable(env, "PATH");
    char **paths = NULL;
    char *path;
    int fd;

    if (index != NULL && (variable_t *){index->data}->value != NULL)
        paths = my_str_to_word_array((variable_t *){index->data}->value, ":");
    for (int i = 0; paths != NULL && paths[i] != NULL; i++){
        path = concat_pathing(cmd, paths[i]);
        fd = open(path, O_RDONLY);
        if (fd != -1){
            free_word_array(paths);
            close(fd);
            return path;
        }
        free(path);
    }
    if (paths != NULL)
        free_word_array(paths);
    return NULL;
}
