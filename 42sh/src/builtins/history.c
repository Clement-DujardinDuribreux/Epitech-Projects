/*
** EPITECH PROJECT, 2025
** history.c
** File description:
** history handling for 42sh
*/

#include "shell.h"

static char *get_command_id(FILE *file, int id)
{
    size_t len = 0;
    char *buffer = NULL;
    char **word_array = NULL;
    char *temp = NULL;

    while (getline(&buffer, &len, file) != -1) {
        buffer[strlen(buffer) - 1] = '\0';
        word_array = my_str_to_word_array(buffer, ":");
        if (word_array && atoi(word_array[0]) == id && word_array[1] != NULL) {
            temp = strdup(word_array[1]);
            free_word_array(word_array);
            fclose(file);
            free(buffer);
            return temp;
        }
        free_word_array(word_array);
    }
    free(buffer);
    fclose(file);
    return NULL;
}

static int is_last_line(char *cmd, linked_list_t *env)
{
    char *path = get_history_path(env);
    int max = 0;
    char *last = NULL;

    if (path == NULL)
        return 0;
    max = get_id(path);
    last = call_history(max, env);
    free(path);
    if (last == NULL)
        return 0;
    if (!strcmp(last, cmd)) {
        free(last);
        return 1;
    }
    free(last);
    return 0;
}

static void process_history_shell(char **word_array, int max_id,
    linked_list_t **env)
{
    char *temp = NULL;
    int cmd_nb = !word_array[0] || !word_array[1] ?
        max_id - 1 : atoi(word_array[1]);
    int a = max_id - cmd_nb > 0 ? max_id - cmd_nb : 1;

    for (; a < max_id; a++) {
        temp = call_history(a, *env);
        printf("%d %s\n", a, temp);
        if (temp != NULL)
            free(temp);
    }
}

static void write_line(int fd, char *temp, char *cmd)
{
    if (temp) {
        write(fd, temp, strlen(temp));
        free(temp);
    }
    write(fd, ":", 1);
    write(fd, cmd, strlen(cmd));
    write(fd, "\n", 1);
}

char *get_history_path(linked_list_t *env)
{
    char *path = NULL;
    linked_list_t *temp = check_env_variable(env, "HOME");
    char *home = NULL;

    if (temp == NULL)
        return NULL;
    home = (variable_t *){temp->data}->value;
    if (home == NULL)
        return NULL;
    path = malloc(sizeof(char) *
        (strlen(home) + strlen("/.42sh_history") + 1));
    if (path == NULL)
        return NULL;
    my_strcpy(path, home);
    my_strcat(path, "/.42sh_history");
    return path;
}

int get_id(char *path)
{
    FILE *file = NULL;
    size_t len = 0;
    char *buffer = NULL;
    char **word_array = NULL;
    int id_max = 0;

    if (path == NULL)
        return 0;
    file = fopen(path, "r");
    if (!file)
        return 0;
    while (getline(&buffer, &len, file) != -1) {
        word_array = my_str_to_word_array(buffer, ":");
        if (word_array && atoi(word_array[0]) > id_max)
            id_max = atoi(word_array[0]);
        free_word_array(word_array);
    }
    free(buffer);
    fclose(file);
    return id_max;
}

void write_history(char *cmd, linked_list_t *env)
{
    char *path = get_history_path(env);
    int fd = 0;
    char *temp = NULL;

    if (path == NULL)
        return;
    if (is_last_line(cmd, env) || cmd == NULL || cmd[0] == '\0') {
        free(path);
        return;
    }
    fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0666);
    if (fd == -1) {
        free(path);
        return;
    }
    temp = int_to_str(get_id(path) + 1);
    free(path);
    write_line(fd, temp, cmd);
    close(fd);
}

char *call_history(int id, linked_list_t *env)
{
    char *path = get_history_path(env);
    FILE *file = NULL;

    if (!path)
        return NULL;
    file = fopen(path, "r");
    if (!file) {
        free(path);
        return NULL;
    }
    free(path);
    return get_command_id(file, id);
}

int history_shell(char *line, shell_var_t *shell_var)
{
    char *path = get_history_path(shell_var->env);
    int max_id = 0;
    char **word_array = my_str_to_word_array(line, " ");

    if (path == NULL)
        return 1;
    if (word_array == NULL) {
        free(path);
        return 1;
    }
    max_id = get_id(path) + 1;
    free(path);
    process_history_shell(word_array, max_id, &shell_var->env);
    free_word_array(word_array);
    return 0;
}
