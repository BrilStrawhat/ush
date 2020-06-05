#include "ush.h"

int mx_check_builtin(st_launch *l_inf, t_shell *shell) {
    for(int i = 0; shell->builtins[i]; i++)
        if (strcmp(shell->builtins[i], l_inf->cmd_arr[0]) == 0) {
            mx_start_builtin(l_inf, shell);
            return 1;
        }
    mx_find_filepath(l_inf->cmd_arr, &l_inf->filepath);
    if (l_inf->filepath == NULL && l_inf->cmd_arr != NULL) {
        for (int i = 0; l_inf->cmd_arr[0][i]; i++) {
            if (l_inf->cmd_arr[0][i] == '/')
                return mx_exec_prog(l_inf, shell);
        }
        mx_printerr("ush: command ");
        mx_printerr(l_inf->cmd_arr[0]);
        mx_printerr(" not found\n");
        return -1;
    }
    return mx_exec_prog(l_inf, shell);
}

int mx_find_filepath(char **cmd_arr, char **filepath) { // rewrite to builtin 'which'
    char *path = mx_strdup(getenv("PATH"));
    char *token = NULL;
    DIR *dptr;
    struct dirent *ds;
    char *temp;

    if (path)
        token = strtok(path, ":"); // need to free token??
    for (; token; token = strtok(NULL, ":")) {
        if ((dptr = opendir(token)) != NULL) {
            for (; (ds = readdir(dptr)) != NULL;) {
                if (strcmp(ds->d_name, cmd_arr[0]) == 0) {
                    temp = mx_strjoin(token, "/");
                    *filepath = mx_strjoin(temp, ds->d_name); // need free in mx_strjoin;
                    mx_strdel(&path);
                    mx_strdel(&temp);
                    closedir(dptr);
                    return 0; // Find file located in PATH
                }
            }
            closedir(dptr);
        }
    }
    return -1;
}
