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
        if (check > 0)
            (*toks)[position++] = mx_trim_token(token);
        if (position >= bufsize)
            auditor(&bufsize, &toks);
        check = mx_check_quotes(line_cp, ' ');
    }
    if (check == -1)
        return;
    else if (check == 100) {
        (*toks)[position] = mx_strdup(mx_trim_token(line_cp));
    }
    else if ((*toks))
        mx_strdel(&line); mx_strdel(&line_cp);
    (*toks)[position + 1] = NULL;
}


static void check_dollar(char *str, int *kk) {
    int i = mx_check_quotes(str, '$');

    if (i > 0 && i != 100) {
        (*kk) += 1;
        check_dollar(&str[i + 1], &(*kk));
    }
    else if (i == 0)
        (*kk) += 1;
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
 mx_printint(kk); // $PWD  return 0 /// need return > 0

    while (kk) {
        char *tmp = NULL;
        tmp = mx_dollar(cmd);////////NEWs
       // mx_printstr(tmp);
        //mx_printchar('\n');
        //mx_strdel(&cmd);
        cmd = mx_strdup(tmp);
        //mx_strdel(&tmp);
        //mx_printstr(cmd);
        //mx_printchar('\n');
        kk--;
}

//mx_printstr(cmd); exit(1);
// create_arr_args
    tokensize(cmd, &command, 64, 0);
    l_inf->cmd_arr = command;
//find cmd
    if (l_inf->cmd_arr)
        mx_check_builtin(l_inf, shell); // 1 = builtin
    // add free alocated memmory because builtin already executed.
    return NULL;
}
