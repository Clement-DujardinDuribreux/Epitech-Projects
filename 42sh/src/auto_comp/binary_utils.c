/*
** EPITECH PROJECT, 2025
** binary_utils.c
** File description:
** binary_utils.c
*/

#include "shell.h"

char **get_path_directories(linked_list_t *env)
{
    variable_t *var;

    for (; env; env = env->next) {
        var = (variable_t *)env->data;
        if (strcmp(var->name, "PATH") == 0)
            return my_str_to_word_array(var->value, ":");
    }
    return NULL;
}

int count_matches_in_directory(char *path, char *prefix)
{
    DIR *dir = opendir(path);
    struct dirent *entry;
    int count = 0;

    if (!dir)
        return 0;
    entry = readdir(dir);
    while (entry != NULL) {
        if (my_strncmp(entry->d_name, prefix, my_strlen(prefix)) == 0 &&
            entry->d_name[0] != '.')
            count++;
        entry = readdir(dir);
    }
    closedir(dir);
    return count;
}

int count_matching_binaries(char **paths, char *prefix)
{
    int match_count = 0;

    for (int i = 0; paths[i]; i++)
        match_count += count_matches_in_directory(paths[i], prefix);
    return match_count;
}

void process_binary_matches(char **paths, char *prefix, char **buffer)
{
    int match_count = count_matching_binaries(paths, prefix);
    char **matches = NULL;

    if (match_count == 0)
        return;
    matches = malloc(sizeof(char *) * (match_count + 1));
    if (!matches)
        return;
    collect_matches(paths, prefix, matches);
    match_count = 0;
    while (matches[match_count] != NULL)
        match_count++;
    handle_matches_unified(matches, match_count, buffer, prefix);
    for (int i = 0; i < match_count; i++)
        free(matches[i]);
    free(matches);
}
