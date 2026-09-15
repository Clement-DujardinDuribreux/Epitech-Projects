/*
** EPITECH PROJECT, 2025
** shell_prompt.c
** File description:
** shell_prompt.c
*/

#include "shell.h"

void get_current_directory(char *cwd, size_t size)
{
    if (getcwd(cwd, size) == NULL) {
        strncpy(cwd, "unknown", size);
        cwd[size - 1] = '\0';
    }
}

void get_system_hostname(char *hostname, size_t size)
{
    if (gethostname(hostname, size) != 0) {
        strncpy(hostname, "unknown", size);
        hostname[size - 1] = '\0';
    }
}

const char *get_username(void)
{
    char *username = getenv("USER");

    if (username)
        return username;
    return "user";
}

void write_colored_text(int fd, const char *color, const char *text)
{
    write(fd, color, strlen(color));
    write(fd, text, strlen(text));
    write(fd, RESET, strlen(RESET));
}

void display_prompt_string(void)
{
    char cwd[255];
    char hostname[256];
    const char *username = get_username();

    if (!isatty(STDIN_FILENO))
        return;
    get_current_directory(cwd, sizeof(cwd));
    get_system_hostname(hostname, sizeof(hostname));
    write(1, YELLOW, strlen(YELLOW));
    write(1, "[", 1);
    write(1, username, strlen(username));
    write(1, "@", 1);
    write(1, hostname, strlen(hostname));
    write(1, " ", 1);
    write_colored_text(1, CYAN_BLUE, cwd);
    write(1, CYAN_BLUE, strlen(CYAN_BLUE));
    write(1, "]$> ", 4);
    write(1, RESET, strlen(RESET));
}
