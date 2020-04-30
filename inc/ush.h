#ifndef USH_H
#define USH_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "libmx.h"
#include <string.h>

#define BUFSIZE 1024

typedef struct s_variables {
    char *name;
    char *value;
    struct s_variables *next;
}                t_variables;    

typedef struct s_shell {
    t_variables *variables;
    t_variables *export_variables;
    char *pwd;
    int status;
}               t_shell;

typedef struct s_cmd {
    char *cmd;
    struct s_cmd *or;
    struct s_cmd *and;
}              t_cmd;

typedef struct s_head {
    struct s_cmd *command;
    struct s_head *next;
}               t_head;


void mx_loop(t_head *head);
t_head *mx_create_head(char *line);
char **mx_tok(char *line);
// void mx_first_cat(char *line);

t_cmd *mx_treefull(char *line);
t_cmd *mx_create_cmd(char *cmd, int i);
void mx_pushtree(t_head **forest, char *tok);
void mx_printforest(t_head *forest);
int mx_check_line(char *line);
char *mx_ex_param(char *line);

// init t_shell info
void mx_init_shell(t_shell *shell);
void mx_push_variable(t_variables **list, void *name, void *value);



#endif
