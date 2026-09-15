/*
** EPITECH PROJECT, 2024
** my_str_isnum
** File description:
** return 1 if str only contains digits else return 0
*/

int my_str_isnum(char const *str)
{
    for (int a = 0; str[a] != '\0'; a++) {
        if (str[a] < 48 || str[a] > 57)
            return 0;
    }
    return 1;
}
