/*
** EPITECH PROJECT, 2024
** my_str_isalpha
** File description:
** return 1 if str is empty or if str contain only
** alphabetical caracters else return 0
*/

int my_str_isalpha(char const *str)
{
    for (int a = 0; str[a] != '\0'; a++) {
        if (str[a] < 65 || str[a] > 90 && str[a] < 97 || str[a] > 122)
            return 0;
    }
    return 1;
}
