/*
** EPITECH PROJECT, 2025
** comp.c
** File description:
** Enhanced auto-completion with improved path support
*/

#include "shell.h"

void list_matching_binaries(char *prefix, linked_list_t *env, char **buffer)
{
    char **paths = get_path_directories(env);

    if (!paths)
        return;
    process_binary_matches(paths, prefix, buffer);
    free_word_array(paths);
}

void auto_comp_handling(char **buffer, linked_list_t **env)
{
    char *partial_path = NULL;
    int last_space = 0;

    if (*buffer == NULL || **buffer == '\0') {
        handle_empty_buffer(buffer);
        return;
    }
    partial_path = get_partial_path(*buffer);
    last_space = get_last_space_position(*buffer);
    if (last_space != -1 && (partial_path == NULL || *partial_path == '\0' ||
        last_space == my_strlen(*buffer) - 1)) {
        handle_empty_buffer(buffer);
        return;
    }
    if (last_space == -1 && !strchr(partial_path, '/')) {
        list_matching_binaries(partial_path, *env, buffer);
        return;
    }
    handle_path_completion(buffer, partial_path);
}
