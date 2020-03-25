#ifndef USH_H
#define USH_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "libmx.h"
#include <string.h>

#define BUFSIZE 1024

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


#endif
