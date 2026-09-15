/*
** EPITECH PROJECT, 2025
** bootstrap
** File description:
** bootstrap
*/
#include "shell.h"

const function_t *functions[8] = {&(function_t){"cd", &cd_shell},
    &(function_t){"setenv", &setenv_shell},
    &(function_t){"local", &local_shell},
    &(function_t){"alias", &alias_shell},
    &(function_t){"unsetenv", &unsetenv_shell},
    &(function_t){"env", &env_shell},
    &(function_t){"history", &history_shell}, NULL};

int switching_process(char *cmd, char **word_array, char **env,
    redirection_t *redirection)
{
    pid_t pid = fork();
    int status;

    if (pid == -1)
        exit(84);
    if (pid == 0){
        if (redirection != NULL)
            set_redirection(redirection);
        if (handle_command(cmd, word_array[0]))
            execve(cmd, word_array, env);
        exit(1);
    } else {
        waitpid(pid, &status, 0);
        if (WTERMSIG(status) == 11)
            write(1, "Segmentation fault\n", 19);
        return WEXITSTATUS(status);
    }
}

static void free_shell_var(shell_var_t *shell_var)
{
    if (shell_var->env != NULL)
        free_list(shell_var->env);
    if (shell_var->local != NULL)
        free_list(shell_var->local);
    if (shell_var->special != NULL)
        free_list(shell_var->special);
    if (shell_var->aliases != NULL)
        free_list(shell_var->aliases);
}

int handling_function(char *line, shell_var_t *shell_var, int return_value)
{
    char **args = my_str_to_word_array(line, " \t");

    if (args != NULL && args[0] != NULL && !my_strcmp(args[0], "exit")){
        free_word_array(args);
        free(line);
        free_shell_var(shell_var);
        exit(return_value);
    }
    for (int i = 0; args != NULL && functions[i] != NULL; i++)
        if (args[0] != NULL && !my_strcmp(args[0], functions[i]->name)){
            free_word_array(args);
            return functions[i]->f(line, shell_var);
        }
    free_word_array(args);
    return -1;
}

static int handle_redirections(char *line, redirection_t *redirection,
    shell_var_t *shell_var)
{
    char **pipes;
    int return_value = 0;

    pipes = my_str_to_word_array(line, "|");
    if (len_word_array(pipes) > 1){
        return_value = handle_pipes(pipes, shell_var, redirection);
    } else {
        return_value = handling_function(line, shell_var, return_value);
        if (return_value < 0) {
            line = get_buffer_alias(line, shell_var->aliases);
            return_value = execute_process(line, shell_var, redirection);
        }
    }
    free_word_array(pipes);
    return return_value;
}

static void handle_command_type(int *return_value,
    char *command, shell_var_t *shell_var)
{
    redirection_t *redirection;

    redirection = get_redirection(command);
    if (redirection != NULL && check_redirection(redirection))
        return;
    if (redirection != NULL)
        *return_value = handle_redirections(redirection->redirected[0],
            redirection, shell_var);
    else
        *return_value = handle_redirections(command,
            redirection, shell_var);
    if (redirection != NULL)
        free_redirect(redirection);
}

static int handle_mandatory(char *command, shell_var_t *shell_var)
{
    char *delimiters[3] = {"&&", "||", NULL};
    char **mandatory = parse_mandatory(command, delimiters);
    int return_value = 0;
    int len = 0;

    for (int i = 0; mandatory[i] != NULL; i++){
        if (i >= 1 && ((!my_strncmp(command + len, "&&", 2) && return_value) ||
            (!my_strncmp(command + len, "||", 2) && !return_value)))
            break;
        if (i >= 1 && !((!my_strncmp(command + len, "&&", 2) &&
            return_value) || (!my_strncmp(command + len, "||", 2) &&
            !return_value)))
            len += 2;
        handle_command_type(&return_value, mandatory[i], shell_var);
        len += my_strlen(mandatory[i]);
    }
    free_word_array(mandatory);
    return return_value;
}

int handle_several_commands(char *line, shell_var_t *shell_var)
{
    char **handle_several_commands = my_str_to_word_array(line, ";");
    int return_value = 0;

    for (int i = 0; handle_several_commands[i] != NULL; i++){
        return_value = handle_mandatory(handle_several_commands[i], shell_var);
    }
    free_word_array(handle_several_commands);
    return return_value;
}

static int display_prompt(shell_var_t *shell_var, int return_value)
{
    struct termios old_term;
    struct termios new_term;

    display_prompt_string();
    setup_terminal(&old_term, &new_term);
    return_value = process_input(shell_var, return_value);
    restore_terminal(&old_term);
    if (return_value == -1)
        return_value = 0;
    return return_value;
}

int main(int ac, char **av, char **env)
{
    shell_var_t shell_var = {.env = set_env_linked_list(env), .local = NULL,
        .special = get_specials_var(ac, av), .aliases = get_base_aliases()};
    int return_value = display_prompt(&shell_var, 0);

    write(1, "\n", 1);
    free_shell_var(&shell_var);
    return return_value;
}
