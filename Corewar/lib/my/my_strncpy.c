/*
** EPITECH PROJECT, 2024
** my_strncpy
** File description:
** copy a str
*/

char *my_strncpy(char *dest, char const *src, int n)
{
    int a = 0;

    for (; src[a] != '\0' && a < n; a++)
        dest[a] = src[a];
    dest[a] = '\0';
    return dest;
}
