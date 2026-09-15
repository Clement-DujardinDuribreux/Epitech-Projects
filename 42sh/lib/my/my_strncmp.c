/*
** EPITECH PROJECT, 2024
** my_strncmp.c
** File description:
** reproduce the behavior of the strncmp function
*/

int my_strncmp(char const *s1, char const *s2, int n)
{
    int i = 0;
    int valeur_s1 = 0;
    int valeur_s2 = 0;

    while (i < n && s1[i] != '\0' && s2[i] != '\0'){
        valeur_s1 += s1[i];
        valeur_s2 += s2[i];
        i++;
    }
    return valeur_s1 - valeur_s2;
}
