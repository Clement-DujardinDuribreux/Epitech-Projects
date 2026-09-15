/*
** EPITECH PROJECT, 2024
** my_strlowcase
** File description:
** put letters in lowcase
*/

char *my_strlowcase(char *str)
{
    for (int a = 0; str[a] != '\0'; a++) {
        if (str[a] > 64 && str[a] < 91)
            str[a] += 32;
    }
    return str;
}
