/*
** EPITECH PROJECT, 2025
** match_output.c
** File description:
** match_output.c
*/

#include "shell.h"

void handle_matches_unified(char **matches, int match_count,
    char **buffer, char *prefix)
{
    if (match_count == 0) {
        supp_buff(*buffer);
        return;
    }
    if (match_count == 1) {
        supp_buff(*buffer);
        complete_exact_match(buffer, matches[0], prefix);
        return;
    }
    sort_matches(matches, match_count);
    write(1, "\n", 1);
    for (int i = 0; i < match_count; i++) {
        write(1, matches[i], my_strlen(matches[i]));
        if (i < match_count - 1)
            write(1, " ", 1);
    }
    write(1, "\n", 1);
    display_prompt_string();
}

void cleanup_resources(DIR *dir, char *dir_path, char *prefix)
{
    if (dir != NULL)
        closedir(dir);
    if (dir_path != NULL)
        free(dir_path);
    if (prefix != NULL)
        free(prefix);
}

void handle_empty_buffer(char **buffer)
{
    handle_directory_completion(buffer, my_strdup("./"), my_strdup(""));
}

void handle_path_completion(char **buffer, char *partial_path)
{
    char *dir_path = get_directory_path(partial_path);
    char *prefix = NULL;

    if (dir_path == NULL) {
        supp_buff(*buffer);
        return;
    }
    prefix = get_filename_prefix(partial_path);
    if (prefix == NULL) {
        supp_buff(*buffer);
        free(dir_path);
        return;
    }
    handle_directory_completion(buffer, dir_path, prefix);
}
