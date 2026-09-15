/*
** EPITECH PROJECT, 2024
** my_getnbr
** File description:
** Get the number in str
*/

int my_getnbr(char const *str)
{
    int nmin = 0;
    int result = 0;

    while (*str != '\0' && ((*str >= 48 && *str <= 57) || result <= 0)) {
        if (*str == '-')
            nmin += 1;
        if (*str >= 48 && *str <= 57)
            result = result * 10 + (*str - 48);
        str++;
    }
    if (nmin % 2 != 0)
        result *= -1;
    return result;
}
