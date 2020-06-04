#include "ush.h"

static void print_env(t_shell *shell) {
    int i = 0;
    if (shell->env == NULL)
        return;
    while (shell->env[i]) {
        mx_printstr(shell->env[i]);
        mx_printchar('\n');
        i++;
    }
}

static void null_env(t_shell *shell) {
    for (int i = 0; shell->env[i]; i++) {
        shell->env[i] = NULL;
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
    while (l_inf->cmd_arr[i - 2]) {
        l_inf->cmd_arr[i - 2] = NULL;
        i++;
    } 
    for(int i = 0; shell->env[i]; i++) {
        shell->env[i] = NULL;
    }
    mx_check_builtin(l_inf, shell);
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
    extern char **environ;

    if (!l_inf->cmd_arr[1])
        print_env(shell);
    else if (strcmp(l_inf->cmd_arr[1], "-u") == 0)
        flag_u(l_inf, shell, l_inf->cmd_arr[2]);
    else if (strcmp(l_inf->cmd_arr[1], "-i") == 0) {
        shell->env = NULL;
        flag_i(l_inf, shell);
    }
    // else if (strcmp(l_inf->cmd_arr[1], "-i") == 0)
    // shell->env = environ;
    return 0;
}
