#include "ush.h"

static void print_env(t_shell *shell) {
    int i = 0;
    while (shell->env[i]) {
        mx_printstr(shell->env[i]);
        mx_printchar('\n');
        i++;
    }
}

static int flag_i(st_launch *l_inf, t_shell *shell) {
    int i = 2; 

    if (!l_inf->cmd_arr[2])
        return 1;
    while (l_inf->cmd_arr[i]) {
        l_inf->cmd_arr[i - 2] = l_inf->cmd_arr[i];
        i++;
    }
    // mx_start_builtin(l_inf, shell);
    return 0;

}

static void flag_u(st_launch *l_inf, t_shell *shell, char *arg) {
    if (!arg)
        mx_printstr("usage: env [-u name]\n");
    else
    for (int i = 0; shell->env[i]; i++) {
        if (strstr(shell->env[i], arg) == 0) {
            mx_printstr(shell->env[i]);
            mx_printchar('\n');
        }
    }
}

int mx_env(st_launch *l_inf, t_shell *shell) {
    char **new_env;

    if (!l_inf->cmd_arr[1]) 
        print_env(shell);
    else if (strcmp(l_inf->cmd_arr[1], "-u") == 0)
        flag_u(l_inf, shell, l_inf->cmd_arr[2]);
    else if (strcmp(l_inf->cmd_arr[1], "-i") == 0)
        flag_i(l_inf, shell);
    else {
        int i = 0;
        while (l_inf->cmd_arr[i]) {
            mx_printstr(l_inf->cmd_arr[i]);
            mx_printchar('\n');
            i++;
        }
    }

    // else if (mx_strcmp(l_inf->cmd_arr[1], "-u"))
    //     flag_u(l_inf, shell, l_inf->cmd_arr[2]);
    //     else
    // return 0;
    return 0;
}
