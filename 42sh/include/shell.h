/*
** EPITECH PROJECT, 2025
** bootstrap
** File description:
** bootstrap
*/

#ifndef SHELL_H
    #define SHELL_H

    #include <unistd.h>
    #include <fcntl.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <stdint.h>
    #include <signal.h>
    #include <stdbool.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <errno.h>
    #include <termios.h>
    #include <dirent.h>
    #include <string.h>
    #include <glob.h>
    #include "my.h"
    #include "list.h"

    #define GREEN "\033[32m"
    #define BLUE "\033[34m"
    #define YELLOW "\033[33m"
    #define CYAN_BLUE "\033[96m"
    #define RESET "\033[0m"

    #define UP 1
    #define DOWN 2
    #define RIGHT 3
    #define LEFT 4

typedef struct variable {
    char *name;
    char *value;
} variable_t;

typedef struct shell_var {
    linked_list_t *env;
    linked_list_t *local;
    linked_list_t *aliases;
    linked_list_t *special;
} shell_var_t;

typedef struct function {
    char *name;
    int (*f)(char *, shell_var_t *);
} function_t;

typedef struct redirection {
    char **redirected;
    char **redirections;
    int nb_redirect;
} redirection_t;

typedef struct pipe {
    int *pipesfd;
    char **piped;
    int len;
    char **env;
    redirection_t *redirect;
} pipe_t;

/* BUILTINS FUNCTIONS */
int env_shell(char *line, shell_var_t *shell_var);
int setenv_shell(char *line, shell_var_t *shell_var);
int unsetenv_shell(char *line, shell_var_t *shell_var);
int cd_shell(char *line, shell_var_t *shell_var);
int local_shell(char *line, shell_var_t *shell_var);
int alias_shell(char *line, shell_var_t *shell_var);
int history_shell(char *line, shell_var_t *shell_var);

/* SHELL FUNCTIONS */
char *pathing(char *cmd, linked_list_t *env);
void set_env(char *name, char *value, linked_list_t **env);
linked_list_t *check_env_variable(linked_list_t *env, char *variable);
char **linked_list_to_word_array(linked_list_t *list);
char *transform_data(variable_t *data);
void my_str_realloc_cat(char **buff, variable_t *str, size_t *len);
linked_list_t *set_env_linked_list(char **env);
variable_t *set_variable(char *name, char *value);
void free_list(linked_list_t *list);
char *get_buffer_alias(char *buff, linked_list_t *aliases);
int handle_command(char *command_value, char *command);
void add_char_to_buffer(char **buffer, size_t *buf_size, char c, bool local);
char *command(char *cmd, linked_list_t *env);
void push_to_list(linked_list_t **begin, void *data);
int is_alphanumeric(char *str);
int execute_process(char *line, shell_var_t *shell_var,
    redirection_t *redirection);
int handle_pipes(char **piped, shell_var_t *shell_var,
    redirection_t *redirect);
char *replace_var(char *buff, linked_list_t *local, bool replace_null);
int *make_pipes(int len);
redirection_t *get_redirection(char *line);
void set_redirection(redirection_t *redirection);
int check_redirection(redirection_t *redirection);
void handle_right_redirect(redirection_t *redirection);
void handle_left_redirect(redirection_t *redirection);
int handling_function(char *line, shell_var_t *shell_var, int return_value);
void free_redirect(redirection_t *redirection);
char **manage_env_separator(char *str, char separator);
void display_prompt_string(void);
void setup_terminal(struct termios *old_term, struct termios *new_term);
void restore_terminal(struct termios *old_term);
int process_input(shell_var_t *shell_var, int return_value);
int handle_several_commands(char *line, shell_var_t *shell_var);
int get_last_space_position(char *buffer);
char *get_partial_path(char *buffer);
void complete_exact_match(char **buffer, char *exact_match, char *prefix);
void swap_strings(char **str1, char **str2);
void sort_matches(char **matches, int match_count);
char *get_directory_path(char *path);
char *get_filename_prefix(char *path);
int is_valid_match(struct dirent *entry, char *prefix);
int is_match_duplicate(char **matches, int match_count, char *name);
int is_match_duplicate_in_list(char **matches, int count, char *name);
void handle_matches_unified(char **matches, int match_count,
    char **buffer, char *prefix);
void cleanup_resources(DIR *dir, char *dir_path, char *prefix);
void handle_empty_buffer(char **buffer);
void handle_path_completion(char **buffer, char *partial_path);
char **get_path_directories(linked_list_t *env);
int count_matches_in_directory(char *path, char *prefix);
int count_matching_binaries(char **paths, char *prefix);
void process_binary_matches(char **paths, char *prefix, char **buffer);
int process_dir_entry(struct dirent *entry, char *prefix,
    char **matches, int *match_count);
int process_directory(char *path, char *prefix,
    char **matches, int *match_count);
int collect_matches(char **paths, char *prefix, char **matches);
char **add_match_to_list(char **matches, int *match_count, char *name);
char **process_dir_entries(DIR *dir, char *prefix, int *match_count);
char **collect_dir_matches(DIR *dir, char *prefix, int *match_count);
void handle_directory_completion(char **buffer, char *dir_path, char *prefix);
void list_matching_binaries(char *prefix, linked_list_t *env, char **buffer);
void auto_comp_handling(char **buffer, linked_list_t **env);
char *get_history_path(linked_list_t *env);
int get_id(char *path);
void write_history(char *cmd, linked_list_t *env);
char *call_history(int id, linked_list_t *env);
void handling_arrows(char **buff, int arrow, int **editing,
    linked_list_t **env);
void supp_buff(char *buff);
void move_cursor(int arrow, int *pos, char *buff);
void reset_cursor(char *buff, int *pos);
int handle_parentheses(char *cmd_parentheses, shell_var_t *shell_var);
char **parse_mandatory(char *str, char **delim);
int is_parentheses_command(const char *command);

int has_glob_pattern(const char *str);
int count_glob_matches(glob_t *globbuf);
int calculate_expanded_size(char **args);
char **init_expanded_args(int size);
int add_regular_string(char **expanded_args, int *expanded_count, char *str);
char **realloc_expanded_args(char **expanded_args, int *expanded_size,
    int expanded_count, int needed_slots);
int add_glob_matches(char **expanded_args, int *expanded_count,
    glob_t *globbuf);
char **process_glob_pattern(char *pattern, char **expanded_args,
    int *expanded_count, int *expanded_size);
static char **process_single_argument(char *arg, char **current_args,
    int *expanded_count, int *expanded_size);
char **process_arguments(char **args, char **expanded_args,
    int *expanded_count, int *expanded_size);
char **expand_glob_patterns(char **args);

int has_glob_pattern(const char *str);
int count_glob_matches(glob_t *globbuf);
int calculate_expanded_size(char **args);
char **init_expanded_args(int size);
int add_regular_string(char **expanded_args, int *expanded_count, char *str);
char **realloc_expanded_args(char **expanded_args, int *expanded_size,
    int expanded_count, int needed_slots);
int add_glob_matches(char **expanded_args, int *expanded_count,
    glob_t *globbuf);
char **process_glob_pattern(char *pattern, char **expanded_args,
    int *expanded_count, int *expanded_size);
static char **process_single_argument(char *arg, char **current_args,
    int *expanded_count, int *expanded_size);
char **expand_glob_patterns(char **args);
int switching_process(char *cmd, char **word_array, char **env,
    redirection_t *redirection);
linked_list_t *get_specials_var(int argc, char **argv);
linked_list_t *get_base_aliases(void);

extern const function_t *functions[];

#endif /* SHELL_H */
