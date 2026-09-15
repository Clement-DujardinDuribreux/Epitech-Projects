/*
** EPITECH PROJECT, 2024
** my_strcmp.c
** File description:
** reproduce the behavior of the strcmp function
*/

int my_strcmp(char const *s1, char const *s2)
{
    int valeur_s1 = 0;
    int valeur_s2 = 0;
    int i = 0;

    while (*s1 == *s2 && *s1 != '\0' && *s2 != '\0'){
        s1++;
        s2++;
    }
    return *s1 - *s2;
}
