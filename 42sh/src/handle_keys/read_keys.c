/*
** EPITECH PROJECT, 2025
** read_keys.c
** File description:
** read_keys.c
*/

#include "shell.h"

static int handle_key(int arrow_key, char c, int *arrow)
{
    if (arrow_key == 0 && c == 27)
        return 1;
    if (arrow_key == 1 && c == 91)
        return 2;
    if (arrow_key == 2 && c == 'A' && isatty(STDIN_FILENO))
        *arrow = UP;
    if (arrow_key == 2 && c == 'B' && isatty(STDIN_FILENO))
        *arrow = DOWN;
    if (arrow_key == 2 && c == 'D' && isatty(STDIN_FILENO))
        *arrow = LEFT;
    if (arrow_key == 2 && c == 'C' && isatty(STDIN_FILENO))
        *arrow = RIGHT;
    return 0;
}

static void handle_backspace(char **buffer, size_t *buf_size, int *pos)
{
    int temp = *pos;

    if (*buf_size > 0 && *pos > 0) {
        reset_cursor(*buffer, pos);
        supp_buff(*buffer);
        (*buf_size)--;
        my_strcpy(&(*buffer)[temp - 1], &(*buffer)[temp]);
        write(1, *buffer, strlen(*buffer));
        *pos = strlen(*buffer);
        for (int a = *pos; a > temp - 1; a--)
            move_cursor(LEFT, pos, *buffer);
    }
}

static int handle_newline(char **buffer,
    shell_var_t *shell_var, int return_value, int *id)
{
    size_t buf_size = 0;

    *id = 0;
    if (*buffer != NULL)
        buf_size = my_strlen(*buffer);
    if (isatty(STDIN_FILENO))
        write(1, "\n", 1);
    if (buf_size > 0 && (*buffer)[0] != '\0')
        return_value = handle_several_commands(*buffer, shell_var);
    free(*buffer);
    *buffer = NULL;
    display_prompt_string();
    return return_value;
}

static void concat_for_edit(char **buffer, char *temp,
    int **pos_temp, size_t *buf_size)
{
    int *pos = pos_temp[0];
    int *temp_pos = pos_temp[1];

    if (*buf_size > 0 && *temp_pos < *buf_size && strlen(temp) > 0) {
        my_strcpy(&(*buffer)[*temp_pos + 1], temp);
        free(temp);
    }
    (*buf_size)++;
    (*buffer)[*buf_size] = '\0';
    (*pos)++;
    if (isatty(STDIN_FILENO)) {
        write(1, *buffer, strlen(*buffer));
        for (int a = strlen(*buffer); a > *temp_pos + 1; a--)
            move_cursor(LEFT, pos, *buffer);
    }
}

static void add_char_to_buffer_entry(char **buffer, size_t *buf_size,
    char c, int *pos)
{
    char *temp = NULL;
    int temp_pos = *pos;

    reset_cursor(*buffer, pos);
    if (*buffer != NULL && isatty(STDIN_FILENO))
        supp_buff(*buffer);
    *buffer = realloc(*buffer, *buf_size + 2);
    if (*buf_size > 0 && temp_pos < *buf_size)
        temp = strdup(&(*buffer)[temp_pos]);
    if (*buf_size > 0 && temp_pos < *buf_size && temp == NULL) {
        *buffer = NULL;
        return;
    }
    (*buffer)[temp_pos] = c;
    concat_for_edit(buffer, temp, (int *[]){pos, &temp_pos}, buf_size);
}

static int process_key(int arrow_key, char c, int *arrow)
{
    if (arrow_key > 0 || c == 27)
        return handle_key(arrow_key, c, arrow);
    return 0;
}

static void process_regular_char(char **buffer, char c, int *pos,
    linked_list_t **env)
{
    size_t buf_size = 0;

    if (*buffer != NULL)
        buf_size = my_strlen(*buffer);
    if (c == '\t') {
        reset_cursor(*buffer, pos);
        auto_comp_handling(buffer, env);
        if (*buffer != NULL) {
            *pos = strlen(*buffer);
            write(1, *buffer, strlen(*buffer));
        }
    } else if (c == 127)
        handle_backspace(buffer, &buf_size, pos);
    else
        add_char_to_buffer_entry(buffer, &buf_size, c, pos);
}

static void change_return_val_special(void *data, void *retval)
{
    variable_t *temp = data;

    if (!my_strcmp(temp->name, "?")) {
        if (temp->value != NULL)
            free(temp->value);
        temp->value = int_to_str(*(int *)retval);
    }
}

static void entry_handling(char c, char **buff, int **editing,
    shell_var_t *shell_var)
{
    if (c == '\n') {
        *(editing[1]) = 0;
        if (*buff != NULL && *buff[0] != '\0')
            write_history(*buff, shell_var->env);
        *buff = replace_var(*buff, shell_var->special, false);
        *buff = replace_var(*buff, shell_var->local, false);
        *buff = replace_var(*buff, shell_var->env, true);
        *(editing[2]) = handle_newline(buff, shell_var,
            *(editing[2]), editing[0]);
        apply_arg(shell_var->special, &change_return_val_special, editing[2]);
    }
}

int process_input(shell_var_t *shell_var, int retval)
{
    char c;
    char *buff = NULL;
    int arrow_key = 0;
    int prev_escape_state = 0;
    int arrow = 0;
    int id = 0;
    int pos = 0;

    while (read(STDIN_FILENO, &c, 1) > 0 && c != 4) {
        prev_escape_state = arrow_key;
        arrow_key = process_key(arrow_key, c, &arrow);
        handling_arrows(&buff, arrow, (int *[]){&id, &pos}, &shell_var->env);
        entry_handling(c, &buff, (int *[]){&id, &pos, &retval}, shell_var);
        if (prev_escape_state == 0 && arrow_key == 0 && c != '\n')
            process_regular_char(&buff, c, &pos, &shell_var->env);
        arrow = 0;
    }
    if (buff != NULL)
        free(buff);
    return retval;
}
