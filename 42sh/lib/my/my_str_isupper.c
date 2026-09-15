/*
** EPITECH PROJECT, 2024
** my_str_isupper
** File description:
** return 1 if str only contains
** uppercase else return 0
*/

int my_str_isupper(char const *str)
{
    for (int a = 0; str[a] != '\0'; a++) {
        if (str[a] < 123 && str[a] > 96)
            return 0;
    }
    return 1;
}
