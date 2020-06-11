#include "ush.h"

static void auditor(int *bufsize, char ****toks) {
    (*bufsize) += 64;
    **toks = realloc((**toks), (*bufsize) * sizeof(char *));
    if (!(**toks)) {
        fprintf(stderr, "u$h: allocation error\n");
        exit(EXIT_FAILURE);
    }
}

static void tokensize(char *line, char ***toks, int bufsize, int position) {
    char *token = NULL;
    char *line_cp = mx_strtrim(line);
    int check = 0;

    check = mx_check_quotes(&line_cp[check], ' ');
    while (check > 0 && check != 100) {
        token = mx_strndup(line_cp, check);
        line_cp = mx_strdup(line_cp + check + 1);
        if (check > 0) { 
            mx_printstr("ENTER 1\n ");
            (*toks)[position++] = mx_trim_token(token);
            mx_printstr((*toks)[position -1]);
        }
        if (position >= bufsize)
            auditor(&bufsize, &toks);
        check = mx_check_quotes(line_cp, ' ');
    }
    if (check == -1)
        return;
    else if (check == 100) {
        mx_printstr("ENTER 2\n ");
        (*toks)[position] = mx_strdup(mx_trim_token(line_cp));
    }
    else if ((*toks))
        mx_strdel(&line); mx_strdel(&line_cp);
    mx_printstr("ENTER 3\n ");
    // mx_printint(position);
    // mx_printstr("\n ");
    (*toks)[position + 1] = NULL;
}


static void check_dollar(char *str, int *kk) {
    int i = mx_check_quotes(str, '$');

    if (i > 0 && i != 100) {
        mx_printint((*kk));
        (*kk) += 1;
        mx_printchar(str[i]);
        //mx_printint(count_dollar);
        mx_printchar('\n');
        check_dollar(&str[i + 1], &(*kk));
    }
    //mx_printint(count_dollar);
    //mx_printchar('\n');
}

st_launch *mx_launch_init(char *cmd, t_shell *shell) {
    if (shell) {

    }
    //int index = -1;
    st_launch *l_inf = malloc(sizeof(st_launch)); // в отдельную функцию 
    l_inf->filepath = NULL;
    l_inf->cmd_arr = NULL;
    l_inf->par = NULL;
    char **command = NULL;
    command = malloc(64 * sizeof(char *));
// exp params
    cmd = mx_ex_param(cmd); // &
    if (!cmd) {
        mx_printerr("error param ${}\n");
        return NULL;
    }

 int kk = 0;
 check_dollar(cmd, &kk);
 mx_printint(kk);

    while (kk) {
        char *tmp = NULL;
        tmp = mx_dollar(cmd);////////NEWs
       // mx_printstr(tmp);
        //mx_printchar('\n');
        mx_strdel(&cmd);
        cmd = mx_strdup(tmp);
        //mx_strdel(&tmp);
        //mx_printstr(cmd);
        //mx_printchar('\n');
        kk--;
}

//mx_printstr(cmd);
//mx_printstr("\n\n\n\n\n");
    mx_printstr("EXIT DOLLAR FUNCTION\n\n");
// create_arr_args
    tokensize(cmd, &command, 64, 0);
     for (int i = 1; i < 3; i++)
         mx_printstr(command[i]);
    l_inf->cmd_arr = command;
//find cmd
    //if (l_inf->cmd_arr)
    //    mx_check_builtin(l_inf, shell); // 1 = builtin
    // add free alocated memmory because builtin already executed.
    return NULL;
}
