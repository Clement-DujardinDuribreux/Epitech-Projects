/*
** EPITECH PROJECT, 2025
** globbing_handler.c
** File description:
** globbing_handler.c
*/

#include "shell.h"

char **process_glob_pattern(char *pattern, char **expanded_args,
    int *expanded_count, int *expanded_size)
{
    glob_t globbuf;
    int result = 0;
    char **new_args;

    result = glob(pattern, GLOB_NOCHECK, NULL, &globbuf);
    if (result != 0)
        return NULL;
    new_args = realloc_expanded_args(expanded_args, expanded_size,
        *expanded_count, globbuf.gl_pathc);
    if (!new_args) {
        globfree(&globbuf);
        return NULL;
    }
    if (add_glob_matches(new_args, expanded_count, &globbuf) == -1)
        return NULL;
    return new_args;
}

int add_glob_matches(char **expanded_args, int *expanded_count,
    glob_t *globbuf)
{
    for (size_t j = 0; j < globbuf->gl_pathc; j++) {
        expanded_args[*expanded_count] = my_strdup(globbuf->gl_pathv[j]);
        if (!expanded_args[*expanded_count]) {
            globfree(globbuf);
            return -1;
        }
        (*expanded_count)++;
    }
    globfree(globbuf);
    return 0;
}

int add_regular_string(char **expanded_args, int *expanded_count, char *str)
{
    expanded_args[*expanded_count] = my_strdup(str);
    if (!expanded_args[*expanded_count])
        return -1;
    (*expanded_count)++;
    return 0;
}

static char **process_single_argument(char *arg, char **current_args,
    int *expanded_count, int *expanded_size)
{
    int result = 0;

    if (has_glob_pattern(arg)) {
        current_args = process_glob_pattern(arg, current_args,
            expanded_count, expanded_size);
        return current_args;
    } else {
        result = add_regular_string(current_args, expanded_count, arg);
        if (result == -1)
            return NULL;
        return current_args;
    }
}

char **process_arguments(char **args, char **expanded_args,
    int *expanded_count, int *expanded_size)
{
    char **current_args = expanded_args;

    for (int i = 0; args[i] != NULL; i++) {
        current_args = process_single_argument(args[i], current_args,
            expanded_count, expanded_size);
        if (!current_args)
            return NULL;
    }
    current_args[*expanded_count] = NULL;
    return current_args;
}

char **expand_glob_patterns(char **args)
{
    int expanded_size = calculate_expanded_size(args);
    char **expanded_args = init_expanded_args(expanded_size);
    int expanded_count = 0;
    char **result_args;

    if (!expanded_args)
        return NULL;
    result_args = process_arguments(args, expanded_args,
        &expanded_count, &expanded_size);
    if (!result_args) {
        free_word_array(expanded_args);
        return NULL;
    }
    return result_args;
}
