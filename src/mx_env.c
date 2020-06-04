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

static int reparse(st_launch *l_inf, t_shell *shell, int n) {
    int i = n; 

    if (!l_inf->cmd_arr[i])
        return 1;
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
    for (int i = 0; shell->env[i]; i++) {
        if (strstr(shell->env[i], arg) == 0) {
            mx_printstr(shell->env[i]);
            mx_printchar('\n');
        }
    }
}

static int flag_P(st_launch *l_inf, t_shell *shell) {
    if (l_inf->cmd_arr[2] && l_inf->cmd_arr[3]) {
        l_inf->filepath = l_inf->cmd_arr[2];
        // mx_printstr(l_inf->filepath);
        // mx_printchar('\n');
        return 2;
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
        shell->env = NULL;
        return 2;
    }
    else if (mx_get_char_index(l_inf->cmd_arr[1], 'P') != -1) {
        return flag_P(l_inf, shell);
    }
    return -1;
}

int mx_env(st_launch *l_inf, t_shell *shell) {
    extern char **environ;
    int n = 0;

    if (!l_inf->cmd_arr[1])
        print_env(shell);
    else {
        n = parse_flags(l_inf, shell, l_inf->cmd_arr[2]);
        if (n > 0 && l_inf->cmd_arr[1])
            reparse(l_inf, shell, n);
    }
    shell->env = environ;
    mx_printstr(l_inf->filepath);
    return 0;
}
