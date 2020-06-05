#include "ush.h"

static void print_env() {
    int i = 0;
    if (environ == NULL)
        return;
    while (environ[i]) {
        mx_printstr(environ[i]);
        mx_printchar('\n');
        i++;
    }
}

static int reparse(st_launch *l_inf, t_shell *shell, int n) {
    int i = n; 

    if (!l_inf->cmd_arr[i])
        return 1;
    for (int j = 0; j < i; j++)
        free(l_inf->cmd_arr[j]);
    while (l_inf->cmd_arr[i]) {
        l_inf->cmd_arr[i - 2] = l_inf->cmd_arr[i];
        i++;
    }
    while (l_inf->cmd_arr[i - 2]) {
        l_inf->cmd_arr[i - 2] = NULL;
        i++;
    }
    mx_check_builtin(l_inf, shell);
    return 0;

}

static void flag_u(st_launch *l_inf, t_shell *shell, char *arg) {
    if (!arg)
        mx_printstr("usage: env [-u name]\n");
    else
    for (int i = 0; environ[i]; i++) {
        if (strstr(environ[i], arg) == 0) {
            mx_printstr(environ[i]);
            mx_printchar('\n');
        }
    }
}

static int flag_P(st_launch *l_inf) {
    char *new_path = getenv("PATH");
    if (l_inf->cmd_arr[2] && l_inf->cmd_arr[3]) {
        setenv("PWD", l_inf->cmd_arr[2], 0);
        // l_inf->filepath = l_inf->cmd_arr[2];
        return 3;
    }
    else
       mx_printstr("usage: env [-P utilpath]\n"); 
    return -1;
}

static int parse_flags(st_launch *l_inf, t_shell *shell, char *arg) {
    if (l_inf->cmd_arr[1][0] != '-')
        return 1;
    if (strcmp(l_inf->cmd_arr[1], "--") == 0) {
        return 2;
    }
    if (mx_get_char_index(l_inf->cmd_arr[1], 'u') != -1) {
        flag_u(l_inf, shell, l_inf->cmd_arr[2]);
    }
    else if (mx_get_char_index(l_inf->cmd_arr[1], 'i') != -1) {
        environ = NULL;
        return 2;
    }
    else if (mx_get_char_index(l_inf->cmd_arr[1], 'P') != -1) {
        return flag_P(l_inf);
    }
    return -1;
}

int mx_env(st_launch *l_inf, t_shell *shell) {
    char **new_env = environ;
    int n = 0;

    if (!l_inf->cmd_arr[1])
        print_env();
    else {
        n = parse_flags(l_inf, shell, l_inf->cmd_arr[2]);
        if (n > 0 && l_inf->cmd_arr[1])
            reparse(l_inf, shell, n);
    }
    environ = new_env;
    mx_printstr(l_inf->filepath);
    return 0;
}
