/*
** EPITECH PROJECT, 2024
** my_str_islower
** File description:
** return 1 if str only contains lowercase else return 0
*/

int my_str_islower(char const *str)
{
    for (int a = 0; str[a] != '\0'; a++) {
        if (str[a] < 91 && str[a] > 64)
            return 0;
    }
    return 1;
}
