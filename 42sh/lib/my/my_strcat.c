/*
** EPITECH PROJECT, 2024
** my_strcat.c
** File description:
** nothing
*/

char *my_strcat(char *dest, char const *src)
{
    int taille_dest = 0;
    int a = 0;

    while (dest[taille_dest] != '\0')
        taille_dest++;
    while (src[a] != '\0'){
        dest[taille_dest + a] = src[a];
        a++;
    }
    dest[taille_dest + a] = '\0';
    return dest;
}
