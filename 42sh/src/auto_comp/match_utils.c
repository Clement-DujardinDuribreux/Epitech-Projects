/*
** EPITECH PROJECT, 2025
** match_utils.c
** File description:
** match_utils.c
*/

#include "shell.h"

int process_dir_entry(struct dirent *entry, char *prefix,
    char **matches, int *match_count)
{
    int len = 0;

    if (strncmp(entry->d_name, prefix, my_strlen(prefix)) == 0 &&
        entry->d_name[0] != '.' &&
        !is_match_duplicate(matches, *match_count, entry->d_name)) {
        matches[*match_count] = my_strdup(entry->d_name);
        (*match_count)++;
        len = my_strlen(entry->d_name);
    }
    return len;
}

int process_directory(char *path, char *prefix,
    char **matches, int *match_count)
{
    DIR *dir = opendir(path);
    struct dirent *entry;
    int max_len = 0;
    int entry_len = 0;

    if (dir == NULL)
        return 0;
    entry = readdir(dir);
    while (entry != NULL) {
        entry_len = process_dir_entry(entry, prefix, matches, match_count);
        if (entry_len > max_len)
            max_len = entry_len;
        entry = readdir(dir);
    }
    closedir(dir);
    return max_len;
}

int collect_matches(char **paths, char *prefix, char **matches)
{
    int match_count = 0;
    int max_len = 0;
    int dir_max_len = 0;

    for (int i = 0; paths[i]; i++) {
        dir_max_len = process_directory(paths[i], prefix,
            matches, &match_count);
        if (dir_max_len > max_len)
            max_len = dir_max_len;
    }
    matches[match_count] = NULL;
    return max_len;
}
