#ifndef USH_H
#define USH_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "libmx.h"
#include <string.h>
#include <sys/types.h>
#include <regex.h>
#include <math.h>
#include <dirent.h>
#include <sys/errno.h>

extern char **environ;

#define BUFSIZE 1024

extern char **environ;

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
    char **builtins;
    char **env;
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

typedef struct s_launch {
    char *filepath; //path to binary;
    char **cmd_arr; // env -ppp
    char **par; // a=b b=c ..
    int type; // no cmd found , builtin, binary      (alias, shell func)
}              st_launch;


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
void mx_tilda(char **line);
void mx_ex_join(char **res, char *full_str, int *r);
int mx_back_slesh(char **str, int* value);
int mx_check_quotes(char *line, char delim);
int mx_cycle_for_quotes(char *line, char delim, int *i);
char *mx_trim_token(char *str);
char **mx_fill_str(char *tok, int count);
void mx_launch_cmd(t_head *forest, t_shell *shell);
st_launch *mx_launch_init(char *cmd, t_shell *shell);

//builtins
int mx_check_builtin(st_launch *l_inf, t_shell *shell);
int mx_find_filepath(char **cmd_arr, char **filepath);
void mx_start(st_launch *l_inf, t_shell *shell); // builtins and path
void mx_start_builtin(st_launch *l_inf, t_shell *shell);

// env
int mx_env(st_launch *l_inf, t_shell *shell);

// init t_shell info
void mx_init_shell(t_shell *shell);
void mx_push_variable(t_variables **list, void *name, void *value);

// fork and exec
int mx_exec_prog(st_launch *l_inf);

// echo
int mx_echo(char **argv, t_shell *shell);

#endif
