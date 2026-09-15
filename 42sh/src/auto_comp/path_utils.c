/*
** EPITECH PROJECT, 2025
** path_utils.c
** File description:
** path_utils.c
*/

#include "shell.h"

char *get_directory_path(char *path)
{
    char *dir_path = NULL;
    int last_slash = -1;
    int i = 0;

    if (path == NULL || path[0] == '\0')
        return my_strdup("./");
    while (path[i] != '\0') {
        if (path[i] == '/')
            last_slash = i;
        i++;
    }
    if (last_slash == -1)
        return my_strdup("./");
    dir_path = malloc(sizeof(char) * (last_slash + 2));
    if (dir_path == NULL)
        return NULL;
    for (i = 0; i <= last_slash; i++)
        dir_path[i] = path[i];
    dir_path[i] = '\0';
    return dir_path;
}

char *get_filename_prefix(char *path)
{
    int last_slash = -1;
    int i = 0;

    if (path == NULL)
        return my_strdup("");
    while (path[i] != '\0') {
        if (path[i] == '/')
            last_slash = i;
        i++;
    }
    return my_strdup(path + last_slash + 1);
}

int is_valid_match(struct dirent *entry, char *prefix)
{
    if (entry->d_name[0] == '.' && (prefix == NULL || prefix[0] != '.'))
        return 0;
    if (prefix == NULL || prefix[0] == '\0')
        return 1;
    return (my_strncmp(entry->d_name, prefix, my_strlen(prefix)) == 0);
}

int is_match_duplicate(char **matches, int match_count, char *name)
{
    for (int j = 0; j < match_count; j++) {
        if (strcmp(matches[j], name) == 0)
            return 1;
    }
    return 0;
}

int is_match_duplicate_in_list(char **matches, int count, char *name)
{
    for (int j = 0; j < count; j++) {
        if (strcmp(matches[j], name) == 0)
            return 1;
    }
    return 0;
}
