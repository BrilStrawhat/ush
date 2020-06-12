#include "ush.h"

// static char *trim_dquotes(const char *str) {
//     int begin = 0;
//     int end = 0;
//     char *result = NULL;

//     if (str == NULL)
//         return NULL;
//     for (begin = 0; str[begin] == '"' && str[begin] != '\0'; begin++)
//     ;
//     for (end = mx_strlen(str); str[end - 1] == '"' && end >= 0; end--)
//     ;

//     for (int i = 0; str[i] == '"'; i++) {
//         if (str[i + 1] == '\0')
//             return NULL;
//     }
//     result = mx_strnew((end - begin) + 1);
//     result = mx_strncpy(result, str + begin, end - begin);
//     return result;
// }


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


st_launch *mx_launch_init(char *cmd, t_shell *shell) {
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
// create_arr_args
    tokensize(cmd, &command, 64, 0);
    mx_open_doll_trim_quotes(&command);



//     for (int k = 0; command[k]; k++) {//////////////////
//             char *str = trim_dquotes(command[k]);
//             mx_strdel(&command[k]);


//  int kk = 0;
//  check_dollar(str, &kk);
//     while (kk) {
//         char *tmp = NULL;
//         tmp = mx_dollar(str);////////NEWs
//        // mx_printstr(tmp);
//         //mx_printchar('\n');
//         //mx_strdel(&cmd);
//         str = mx_strdup(tmp);
//         //mx_strdel(&tmp);
//         //mx_printstr(cmd);
//         //mx_printchar('\n');
//         kk--;
// }
//             command[k] = mx_strdup(str);
//             mx_strdel(&str);
//         }//


    l_inf->cmd_arr = command;
//find cmd 
    if (l_inf->cmd_arr)
        mx_check_builtin(l_inf, shell); // 1 = builtin
    // add free alocated memmory because builtin already executed.
    //mx_printstr(l_inf->cmd_arr[0]);
    return NULL;
}
