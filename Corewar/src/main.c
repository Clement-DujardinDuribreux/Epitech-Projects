/*
** EPITECH PROJECT, 2025
** main.c
** File description:
** main file for Corewar
*/

#include "corewar.h"

static void exec_instruction(robot_t *robot)
{
    const void (*f[])(robot_t *) = {&live_exe, &ld_exe, &st_exe, &add_exe,
        &sub_exe, &and_exe, &or_exe, &xor_exe, &zjmp_exe, &ldi_exe, &sti_exe,
        &fork_exe, &lld_exe, &lldi_exe, &lfork_exe, &aff_exe};

    robot->count = 0;
    if (robot->code_instruction > 16 || robot->code_instruction < 1)
        return;
    f[robot->code_instruction - 1](robot);
    robot->code_instruction = 0;
    robot->pos = (robot->pos + robot->nb_bytes_args + 1) % MEM_SIZE;
    robot->nb_bytes_args = 0;
}

static void get_instruction(robot_t *robot)
{
    const void (*f[])(robot_t *) = {&live_cor, &ld_cor, &st_cor, &add_cor,
        &sub_cor, &and_cor, &or_cor, &xor_cor, &zjmp_cor, &ldi_cor, &sti_cor,
        &fork_cor, &lld_cor, &lldi_cor, &lfork_cor, &aff_cor};

    exec_instruction(robot);
    reset_args(robot->args);
    robot->code_instruction = robot->area[robot->pos];
    if (robot->code_instruction > 16 || robot->code_instruction < 1) {
        robot->need = 1;
        robot->pos = (robot->pos + 1) % MEM_SIZE;
        return;
    }
    if (robot->code_instruction != 1 && robot->code_instruction != 9 &&
        robot->code_instruction != 12 && robot->code_instruction != 15)
            get_args_types(robot->args_type, robot->area[robot->pos + 1]);
    f[robot->code_instruction - 1](robot);
    get_args(robot->args, robot->area, robot->pos + 1, robot->nb_bytes_args);
    robot->need = op_tab[robot->code_instruction - 1].nbr_cycles;
    return;
}

static void init_champ(robot_t *champ, robot_init_t *init, char *area)
{
    char prog_size[4];

    fseek(init->file, sizeof(int), SEEK_SET);
    fread(&champ->name, PROG_NAME_LENGTH + 4, 1, init->file);
    fread(&prog_size, 4, 1, init->file);
    fseek(init->file, sizeof(header_t), SEEK_SET);
    champ->area = area;
    champ->pos = init->address;
    fread(&area[champ->pos], get_int(prog_size, 4), 1, init->file);
    champ->id = init->id;
    champ->carry = 1;
    champ->count = 0;
    champ->need = 0;
    champ->is_alive = 1;
    champ->alive_count = -1;
    convert_int_to_hex(init->id, champ->reg[0]);
}

static robot_t **get_champs(robot_init_t **list, char *area)
{
    robot_t **champs = malloc(sizeof(robot_t *) * (nb_robots_init(list) + 1));

    for (int i = 0; champs != NULL && list[i] != NULL; i++) {
        champs[i] = malloc(sizeof(robot_t));
        if (champs[i] == NULL)
            return NULL;
        init_champ(champs[i], list[i], area);
        champs[i]->childs = NULL;
        champs[i]->parent = NULL;
        champs[i]->nb_bytes_args = 0;
        champs[i]->code_instruction = 0;
        champs[i]->live_id = 0;
        reset_args(champs[i]->args);
        champs[i + 1] = NULL;
    }
    return champs;
}

static void set_alive(int id, robot_t **robot)
{
    for (int a = 0; robot != NULL && robot[a] != NULL; a++) {
        if (robot[a]->id == id) {
            robot[a]->alive_count = 0;
            set_alive(id, robot[a]->childs);
        }
        if (robot[a]->id == id && robot[a]->parent == NULL) {
            write(1, "The player ", 11);
            my_put_nbr(robot[a]->id);
            write(1, " (", 2);
            write(1, robot[a]->name, my_strlen(robot[a]->name));
            write(1, "): is alive !\n", 14);
        }
    }
}

static int all_alive(robot_t **robot)
{
    int nbr_alive = 0;

    for (int a = 0; robot != NULL && robot[a] != NULL; a++) {
        if (robot[a]->live_id != 0)
            set_alive(robot[a]->live_id, robot);
        robot[a]->live_id = 0;
        if (robot[a]->alive_count >= CYCLE_TO_DIE)
            robot[a]->is_alive = 0;
        else
            nbr_alive++;
    }
    return nbr_alive > 1;
}

static void champs_instructions(robot_t **champs)
{
    for (int a = 0; champs != NULL && champs[a] != NULL; a++) {
        if (champs[a]->count >= champs[a]->need && champs[a]->is_alive)
            get_instruction(champs[a]);
        if (champs[a]->is_alive && champs[a]->childs != NULL)
            champs_instructions(champs[a]->childs);
        champs[a]->count++;
        champs[a]->alive_count++;
    }
}

static void write_winner(robot_t **champs)
{
    for (int a = 0; champs[a] != NULL; a++)
        if (champs[a]->is_alive) {
            write(1, "The player ", 11);
            my_put_nbr(champs[a]->id);
            write(1, " (", 2);
            write(1, champs[a]->name, my_strlen(champs[a]->name));
            write(1, "): has won ! GG\n", 16);
            return;
        }
}

void write_map(char area[MEM_SIZE])
{
    char *temp = NULL;

    for (int a = 0; a < MEM_SIZE; a++) {
        temp = decimal_to_base("0123456789abcdef", area[a]);
        if (temp != NULL) {
            write(1, temp, 2);
            free(temp);
            write(1, " ", 1);
        }
    }
    write(1, "\n", 1);
}

int main(int argc, char *argv[])
{
    int cycle = 0;
    robot_t **champs = NULL;
    char *area = malloc(sizeof(char) * MEM_SIZE);
    robot_init_t **list = get_init_robot(argc, argv);

    if (list == NULL)
        return 1;
    reset_map(area);
    champs = get_champs(list, area);
    while (all_alive(champs)) {
        champs_instructions(champs);
        cycle++;
        if (get_dump(argc, argv) && cycle % get_dump(argc, argv) == 0)
            write_map(area);
    }
    write_winner(champs);
    free_champs(champs);
    free(area);
    free_robot_init(list);
    return 0;
}
