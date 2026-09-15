/*
** EPITECH PROJECT, 2025
** pipe
** File description:
** pipe
*/
#include "shell.h"

int *make_pipes(int len)
{
    int *pipes = malloc(sizeof(int) * (len - 1) * 2);

    if (pipes == NULL)
        return NULL;
    for (int i = 0; i < len - 1; i++){
        if (pipe(pipes + i * 2) == -1){
            free(pipes);
            return NULL;
        }
    }
    return pipes;
}

void set_dup(int index, pipe_t *pipe)
{
    if (index != 0){
        dup2(pipe->pipesfd[(index - 1) * 2], STDIN_FILENO);
    } else if (pipe->redirect != NULL)
        handle_left_redirect(pipe->redirect);
    if (index != pipe->len - 1){
        dup2(pipe->pipesfd[index * 2 + 1], STDOUT_FILENO);
    } else if (pipe->redirect != NULL)
        handle_right_redirect(pipe->redirect);
    for (int i = 0; i < 2 * (pipe->len - 1); i++)
        close(pipe->pipesfd[i]);
}

static void handle_builtin(pipe_t *pipe, shell_var_t *shell_var, int index)
{
    char **word_array;
    char *cmd;
    int return_value = 1;

    if (handling_function(pipe->piped[index], shell_var, 0) < 0){
        if (is_parentheses_command(pipe->piped[index])) {
            return_value = handle_parentheses(pipe->piped[index], shell_var);
            exit(return_value);
        }
        word_array = my_str_to_word_array(pipe->piped[index], " \t");
        cmd = command(*word_array, shell_var->env);
        if (word_array != NULL && handle_command(cmd, word_array[0]))
            execve(cmd, word_array, pipe->env);
        exit(1);
    }
    exit(0);
}

void exec_childs(pipe_t *pipe, shell_var_t *shell_var)
{
    pid_t pid;

    for (int i = 0; i < pipe->len; i++){
        pid = fork();
        if (pid == 0){
            set_dup(i, pipe);
            handle_builtin(pipe, shell_var, i);
        }
    }
    for (int i = 0; i < 2 * (pipe->len - 1); i++)
        close(pipe->pipesfd[i]);
    free(pipe->pipesfd);
    free_word_array(pipe->env);
    for (int i = 0; i < pipe->len; i++)
        wait(NULL);
}

int handle_pipes(char **piped, shell_var_t *shell_var, redirection_t *redirect)
{
    pipe_t pipe;

    pipe.piped = piped;
    pipe.len = len_word_array(piped);
    pipe.env = linked_list_to_word_array(shell_var->env);
    pipe.pipesfd = make_pipes(pipe.len);
    pipe.redirect = redirect;
    if (pipe.pipesfd != NULL)
        exec_childs(&pipe, shell_var);
    return 0;
}
