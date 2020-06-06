#include "ush.h"

static int parse_flags(st_launch *l_inf) {
    if (!l_inf->cmd_arr[1] || strcmp(l_inf->cmd_arr[1], "--") == 0)
        return 0;
    if (l_inf->cmd_arr[1][0] != '-') {
        if (mx_get_char_index(l_inf->cmd_arr[1], 'P') != -1) {

        }
        if (mx_get_char_index(l_inf->cmd_arr[1], 'L') != -1) {

        }

        return 1;
    }
    else 
        mx_printerr("pwd: bad option\n");

    return 1;
}

int mx_pwd(st_launch *l_inf) {
    char *pwd = NULL;
    char *realpath;

    if (parse_flags(l_inf)) {
        mx_printstr("flags pwd \n");
    }
    else {
        pwd = getenv("PWD");
        mx_printstr(pwd);
        mx_printchar('\n');
    }
    // char *oldpwd = getenv("OLDPWD");
    // mx_printstr(pwd);

    return 1;
}
