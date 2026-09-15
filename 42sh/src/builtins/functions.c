/*
** EPITECH PROJECT, 2025
** functions
** File description:
** functions
*/
#include "shell.h"

static void change_cwd_special(void *data)
{
    variable_t *temp = data;

    if (!my_strcmp(temp->name, "cwd")) {
        if (temp->value != NULL)
            free(temp->value);
        temp->value = getcwd(NULL, 256);
    }
}

static void change_variable_cd_shell(linked_list_t **env,
    linked_list_t *special)
{
    linked_list_t *pwd = check_env_variable(*env, "PWD");
    char *current;

    if (pwd != NULL)
        set_env("OLDPWD", (variable_t *){pwd->data}->value, env);
    current = getcwd(NULL, 256);
    apply(special, &change_cwd_special);
    set_env("PWD", current, env);
    free(current);
}

static int handle_cd_error(int cd_value, char *directory)
{
    if (cd_value < 0){
        write(1, directory, my_strlen(directory));
        if (!access(directory, F_OK)){
            write(1, ": Not  a directory\n", 19);
        } else
            write(1, ": No such file or directory.\n", 30);
        return 1;
    }
    return 0;
}

static int handle_specific_cd(char **args, linked_list_t *env)
{
    linked_list_t *oldpwd = check_env_variable(env, "OLDPWD");
    linked_list_t *home = check_env_variable(env, "HOME");

    if (len_word_array(args) == 1 || !my_strcmp(args[1], "~")){
        if (home != NULL)
            return chdir((variable_t *){home->data}->value);
        else {
            write(1, "", 1);
            return -1;
        }
    }
    if (!my_strcmp(args[1], "-")){
        if (oldpwd != NULL)
            return chdir((variable_t *){oldpwd->data}->value);
        else {
            write(1, "", 1);
            return -1;
        }
    }
    return 0;
}

int cd_shell(char *line, shell_var_t *shell_var)
{
    char **args = my_str_to_word_array(line, " \t");
    int cd_value;

    if (len_word_array(args) > 2){
        write(1, "Too many arguments.\n", 20);
        free_word_array(args);
        return 1;
    }
    if (len_word_array(args) == 1 || !my_strcmp(args[1], "~") ||
        !my_strcmp(args[1], "-"))
        cd_value = handle_specific_cd(args, shell_var->env);
    else
        cd_value = chdir(args[1]);
    if (handle_cd_error(cd_value, args[1])){
        free_word_array(args);
        return 1;
    }
    change_variable_cd_shell(&shell_var->env, shell_var->special);
    free_word_array(args);
    return 0;
}

void set_env(char *name, char *value, linked_list_t **env)
{
    linked_list_t *env_line = check_env_variable(*env, name);

    if (env_line == NULL){
        push_to_list(env, set_variable(name, value));
    } else {
        free((variable_t *){env_line->data}->value);
        (variable_t *){env_line->data}->value = my_strdup(value);
    }
}

int local_shell(char *line, shell_var_t *shell_var)
{
    char **word_array = my_str_to_word_array(line, " \t");
    variable_t *data = NULL;

    for (int i = 0; word_array[i] != NULL; i++);
    if (word_array == NULL)
        return 1;
    data = malloc(sizeof(variable_t));
    if (data == NULL)
        return 1;
    data->name = word_array[1];
    data->value = word_array[2];
    shell_var->local = add(shell_var->local, data);
    return 0;
}
