/*
** EPITECH PROJECT, 2025
** redirect
** File description:
** redirect
*/
#include "shell.h"

static void set_redirections_array(redirection_t *redirect, char *line)
{
    int len = 0;
    char *redirection = NULL;

    for (int i = 0; i < redirect->nb_redirect; i++){
        len += my_strlen(redirect->redirected[i]);
        if (!my_strncmp(line + len, ">", 1))
            redirection = ">";
        if (!my_strncmp(line + len, ">>", 2))
            redirection = ">>";
        if (!my_strncmp(line + len, "<", 1))
            redirection = "<";
        if (!my_strncmp(line + len, "<<", 2))
            redirection = "<<";
        len += my_strlen(redirection);
        redirect->redirections[i] = my_strdup(redirection);
    }
    redirect->redirections[redirect->nb_redirect] = NULL;
}

redirection_t *get_redirection(char *line)
{
    redirection_t *new_redirect = malloc(sizeof(*new_redirect));

    if (new_redirect == NULL)
        return NULL;
    new_redirect->redirected = my_str_to_word_array(line, "><");
    new_redirect->nb_redirect = len_word_array(new_redirect->redirected) - 1;
    if (new_redirect->nb_redirect <= 0){
        free_word_array(new_redirect->redirected);
        free(new_redirect);
        return NULL;
    }
    new_redirect->redirections = malloc(sizeof(char *) *
        (new_redirect->nb_redirect + 1));
    if (new_redirect->redirections == NULL){
        free_word_array(new_redirect->redirected);
        free(new_redirect);
        return NULL;
    }
    set_redirections_array(new_redirect, line);
    return new_redirect;
}

static void set_right_redirection(redirection_t *redirection,
    int index, int flag)
{
    char **clean_str;
    int file;

    clean_str = my_str_to_word_array(redirection->redirected[index + 1],
        " \t");
    file = open(clean_str[0], O_CREAT | flag | O_WRONLY, 0664);
    if (file != -1){
        dup2(file, STDOUT_FILENO);
        close(file);
    }
    free_word_array(clean_str);
}

static void set_left_redirection(redirection_t *redirection, int index)
{
    char **clean_str;
    int file;

    clean_str = my_str_to_word_array(redirection->redirected[index + 1],
        " \t");
    file = open(clean_str[0], O_RDONLY);
    if (file != -1){
        dup2(file, STDIN_FILENO);
        close(file);
    }
    free_word_array(clean_str);
}

int check_redirection(redirection_t *redirection)
{
    int left = 0;
    int right = 0;

    for (int i = 0; redirection->redirections[i] != NULL; i++){
        if (!my_strcmp(redirection->redirections[i], ">>") ||
            !my_strcmp(redirection->redirections[i], ">"))
            right++;
        if (!my_strcmp(redirection->redirections[i], "<<") ||
            !my_strcmp(redirection->redirections[i], "<"))
            left++;
    }
    if (left > 1){
        write(1, "amiguous input redirect.\n", 25);
        return 1;
    }
    if (right > 1){
        write(1, "amiguous output redirect.\n", 26);
        return 1;
    }
    return 0;
}

void handle_right_redirect(redirection_t *redirection)
{
    for (int i = 0; redirection->redirections[i] != NULL; i++){
        if (redirection->redirected[i + 1] == NULL)
            exit(1);
        if (!my_strcmp(redirection->redirections[i], ">"))
            set_right_redirection(redirection, i, O_TRUNC);
        if (!my_strcmp(redirection->redirections[i], ">>"))
            set_right_redirection(redirection, i, O_APPEND);
    }
}

static void handle_heredoc(redirection_t *redirection, int index)
{
    int pipefd[2];
    char *line = NULL;
    char **clean_str;

    if (pipe(pipefd) == -1)
        return;
    clean_str = my_str_to_word_array(redirection->redirected[index + 1],
        " \t");
    write(1, "heredoc> ", 10);
    while (getline(&line, &(size_t){1}, stdin) != -1){
        if (!my_strncmp(line, *clean_str, my_strlen(line) - 1))
            break;
        write(pipefd[1], line, my_strlen(line));
        write(1, "heredoc> ", 10);
    }
    free_word_array(clean_str);
    dup2(pipefd[0], STDIN_FILENO);
    for (int i = 0; i < 2; i++)
        close(pipefd[i]);
}

void handle_left_redirect(redirection_t *redirection)
{
    for (int i = 0; redirection->redirections[i] != NULL; i++){
        if (redirection->redirected[i + 1] == NULL)
            exit(1);
        if (!my_strcmp(redirection->redirections[i], "<"))
            set_left_redirection(redirection, i);
        if (!my_strcmp(redirection->redirections[i], "<<"))
            handle_heredoc(redirection, i);
    }
}

void set_redirection(redirection_t *redirection)
{
    handle_left_redirect(redirection);
    handle_right_redirect(redirection);
}
