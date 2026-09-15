/*
** EPITECH PROJECT, 2025
** line_edition.c
** File description:
** line_edition.c
*/

#include "shell.h"

static int utf8_strlen(const char *src)
{
    int len = 0;

    while (*src != '\0') {
        *src++;
        if ((*src & 0xC0) != 0x80)
            len++;
    }
    return len;
}

void supp_buff(char *buff)
{
    for (int a = utf8_strlen(buff); a > 0; a--)
        write(1, "\b \b", 3);
}

void move_cursor(int arrow, int *pos, char *buff)
{
    if (arrow == LEFT && *pos > 0) {
        printf("\033[%dD", 1);
        (*pos)--;
    }
    if (arrow == RIGHT && buff != NULL && *pos < strlen(buff)) {
        printf("\033[%dC", 1);
        (*pos)++;
    }
    fflush(stdout);
}

void reset_cursor(char *buff, int *pos)
{
    if (buff == NULL)
        return;
    for (int a = *pos; a < strlen(buff); a++)
        move_cursor(RIGHT, pos, buff);
}
