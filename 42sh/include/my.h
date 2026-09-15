/*
** EPITECH PROJECT, 2024
** my.h
** File description:
** include lib/my
*/

#ifndef MY
    #define MY

int my_put_nbr(int nb);
int my_strlen(char const *str);
int my_getnbr(char const *str);
int my_compute_power_rec(int nb, int power);
char *my_strcpy(char *dest, char const *src);
char *my_strncpy(char *dest, char const *src, int n);
char *my_revstr(char *str);
int my_strstr(char *s1, char const *s2);
int my_strcmp(char const *s1, char const *s2);
int my_strncmp(char const *s1, char const *s2, int n);
char *my_strupcase(char *str);
char *my_strlowcase(char *str);
int my_str_isalpha(char const *str);
int my_str_isnum(char const *str);
int my_str_islower(char const *str);
int my_str_isupper(char const *str);
char *my_strcat(char *dest, char const *src);
char *my_strncat(char *dest, char const *src, int nb);
char **my_str_to_word_array(char const *str, char *separator);
void free_word_array(char **word_array);
char *my_strdup(char const *src);
char *append(char *dest, char c);
char *int_to_str(int nb);
void double_to_str(char *dest, double nb, int nb_decimal);
int in_word_array(char **word_array, char *find);
int len_word_array(char **word_array);
int is_letter(char c);

#endif /* MY */
