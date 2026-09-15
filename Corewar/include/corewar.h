/*
** EPITECH PROJECT, 2024
** list.h
** File description:
** include lib/list
*/

#ifndef COREWAR
    #define COREWAR

    #include <stdlib.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include "op.h"
    #include "my.h"
    #include "list.h"

    #define MAX_LENGHT_ARGS DIR_SIZE * MAX_ARGS_NUMBER

typedef struct robot_s {
    int id;
    char name[PROG_NAME_LENGTH];
    char *area;
    int pos;
    int code_instruction;
    int nb_bytes_args;
    char args[MAX_LENGHT_ARGS];
    args_type_t args_type[MAX_ARGS_NUMBER];
    int carry;
    int need;
    int count;
    int is_alive;
    int alive_count;
    int live_id;
    char reg[REG_NUMBER][REG_SIZE];
    struct robot_s *parent;
    struct robot_s **childs;
} robot_t;

typedef struct robot_init_s {
    FILE *file;
    int id;
    int address;
} robot_init_t;

robot_init_t **get_init_robot(int argc, char *argv[]);
void get_bytes(int *instructions, int nb, FILE *file);
void get_args(char args[MAX_LENGHT_ARGS], char *area, int pos, int nb);
int get_int(char *hexa, int nb_bytes);
void convert_int_to_hex(int dec, char *hex);
void free_champs(robot_t **champs);
void free_robot_init(robot_init_t **list);
char get_type(char binary);
void cpy_robot(robot_t *src, robot_t *dest);
void get_args_types(args_type_t args_types[MAX_ARGS_NUMBER],
    unsigned char coding_byte);
int robot_reg(robot_t *robot, int i, int value[2], int index);
int nb_robots_init(robot_init_t **list);
void reset_args(char args[MAX_LENGHT_ARGS]);
void reset_args_types(args_type_t types[MAX_ARGS_NUMBER]);
void reset_reg(char reg[REG_NUMBER][REG_SIZE]);
int get_dump(int argc, char *argv[]);
void reset_map(char map[MEM_SIZE]);
char *decimal_to_base(char *base, unsigned char nb);

/* INSTRUCTIONS GET */
void live_cor(robot_t *robot);
void ld_cor(robot_t *robot);
void st_cor(robot_t *robot);
void add_cor(robot_t *robot);
void sub_cor(robot_t *robot);
void and_cor(robot_t *robot);
void or_cor(robot_t *robot);
void xor_cor(robot_t *robot);
void zjmp_cor(robot_t *robot);
void ldi_cor(robot_t *robot);
void sti_cor(robot_t *robot);
void fork_cor(robot_t *robot);
void lld_cor(robot_t *robot);
void lldi_cor(robot_t *robot);
void lfork_cor(robot_t *robot);
void aff_cor(robot_t *robot);

/* INSTRUCTIONS EXE */
void live_exe(robot_t *robot);
void ld_exe(robot_t *robot);
void st_exe(robot_t *robot);
void add_exe(robot_t *robot);
void sub_exe(robot_t *robot);
void and_exe(robot_t *robot);
void or_exe(robot_t *robot);
void xor_exe(robot_t *robot);
void zjmp_exe(robot_t *robot);
void ldi_exe(robot_t *robot);
void sti_exe(robot_t *robot);
void fork_exe(robot_t *robot);
void lld_exe(robot_t *robot);
void lldi_exe(robot_t *robot);
void lfork_exe(robot_t *robot);
void aff_exe(robot_t *robot);

#endif /* COREWAR */
