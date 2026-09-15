/*
** EPITECH PROJECT, 2024
** my_str_to_word_array.c
** File description:
** my_str_to_word_array
*/
#include <stdlib.h>
#include <unistd.h>

static int isnt_delimiter(char c, char *delimiters)
{
    for (int i = 0; delimiters[i] != '\0'; i++){
        if (c == delimiters[i])
            return 0;
    }
    return 1;
}

static int get_nb_word(char const *str, char *delimiters)
{
    int nb = 0;
    int in_quote = 0;
    int in_parentheses = 0;

    for (int i = 0; str[i] != '\0'; i++){
        if (!in_parentheses && !in_quote && isnt_delimiter(str[i], delimiters))
            nb++;
        if (str[i] == '"')
            in_quote = !in_quote;
        if (str[i] == '(' && !in_quote)
            in_parentheses = 1;
        if (str[i] == ')' && !in_quote)
            in_parentheses = 0;
    }
    return nb;
}

static int get_number_char(char const *str, char *delimiters)
{
    int nb = 0;
    int quote = 0;
    int parentheses = 0;

    for (int x = 0; str[x] != '\0'; x++){
        if (str[x] == '(' && !quote)
            parentheses = 1;
        if (!quote && !parentheses && !isnt_delimiter(str[x], delimiters))
            break;
        if (quote && str[x] == '"' && str[0] == '"')
            break;
        if (str[x] == '"')
            quote = !quote;
        nb++;
        if (parentheses && str[x] == ')' && !quote)
            break;
    }
    return nb;
}

static char *put_str_temp(char const *str, char *delimiters)
{
    char *str_temp;
    int nb = get_number_char(str, delimiters);

    if (str[0] == '"'){
        str++;
        nb--;
    }
    str_temp = malloc(sizeof(char) * (nb + 1));
    if (str_temp == NULL)
        return NULL;
    for (int i = 0; str[i] != '\0' && i < nb; i++)
        str_temp[i] = str[i];
    str_temp[nb] = '\0';
    return str_temp;
}

static void update_state(char const *str, int *quote,
    int *parentheses, int end)
{
    if (end == 0){
        if (str[0] == '(' && !(int){*quote})
            *parentheses = 1;
    } else {
        if (str[0] == ')' && !(int){*quote})
            *parentheses = 0;
        if (str[0] == '"')
            *quote = !(int){*quote};
    }
}

char **my_str_to_word_array(char const *str, char *delim)
{
    char **word_array = malloc(sizeof(char *) * (get_nb_word(str, delim) + 1));
    int can = 1;
    int index = 0;
    int in_quote = 0;
    int in_parentheses = 0;

    for (int i = 0; word_array != NULL && str[i] != '\0'; i++){
        update_state(str + i, &in_quote, &in_parentheses, 0);
        if (!isnt_delimiter(str[i], delim) && !in_quote && !in_parentheses)
            can = 1;
        if (can && isnt_delimiter(str[i], delim)){
            word_array[index] = put_str_temp(str + i, delim);
            can = 0;
            index++;
        }
        update_state(str + i, &in_quote, &in_parentheses, 1);
    }
    if (word_array != NULL)
        word_array[index] = NULL;
    return word_array;
}

void free_word_array(char **word_array)
{
    for (int i = 0; word_array[i] != NULL; i++){
        free(word_array[i]);
    }
    free(word_array);
}

int len_word_array(char **word_array)
{
    int len = 0;

    for (int i = 0; word_array[i] != NULL; i++)
        len++;
    return len;
}
