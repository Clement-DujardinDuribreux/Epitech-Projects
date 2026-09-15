/*
** EPITECH PROJECT, 2024
** my_strupcase
** File description:
** put every letters in upper case
*/

char *my_strupcase(char *str)
{
    for (int a = 0; str[a] != '\0'; a++) {
        if (str[a] > 96 && str[a] < 123)
            str[a] -= 32;
    }
    return str;
}
