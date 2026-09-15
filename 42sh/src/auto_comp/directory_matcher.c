/*
** EPITECH PROJECT, 2025
** directory_matcher.c
** File description:
** directory_matcher.c
*/

#include "shell.h"

char **add_match_to_list(char **matches, int *match_count, char *name)
{
    char **new_matches = realloc(matches, sizeof(char *) * (*match_count + 2));

    if (new_matches == NULL)
        return matches;
    new_matches[*match_count] = my_strdup(name);
    (*match_count)++;
    return new_matches;
}

char **process_dir_entries(DIR *dir, char *prefix, int *match_count)
{
    struct dirent *entry;
    char **matches = NULL;

    *match_count = 0;
    entry = readdir(dir);
    while (entry != NULL) {
        if (is_valid_match(entry, prefix) &&
            !is_match_duplicate_in_list(matches, *match_count, entry->d_name))
            matches = add_match_to_list(matches, match_count, entry->d_name);
        entry = readdir(dir);
    }
    return matches;
}

char **collect_dir_matches(DIR *dir, char *prefix, int *match_count)
{
    char **matches = NULL;

    rewinddir(dir);
    matches = process_dir_entries(dir, prefix, match_count);
    if (matches)
        matches[*match_count] = NULL;
    return matches;
}

void handle_directory_completion(char **buffer, char *dir_path, char *prefix)
{
    DIR *dir = opendir(dir_path);
    int match_count = 0;
    char **matches = NULL;

    if (dir == NULL) {
        supp_buff(*buffer);
        free(dir_path);
        free(prefix);
        return;
    }
    matches = collect_dir_matches(dir, prefix, &match_count);
    if (matches != NULL && match_count > 0) {
        handle_matches_unified(matches, match_count, buffer, prefix);
        for (int i = 0; i < match_count; i++)
            free(matches[i]);
        free(matches);
    } else
        supp_buff(*buffer);
    cleanup_resources(dir, dir_path, prefix);
}
