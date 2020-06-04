#include "ush.h"

int mx_check_builtin(st_launch *l_inf, t_shell *shell) {
    for(int i = 0; shell->builtins[i]; i++)
        if (strcmp(shell->builtins[i], l_inf->cmd_arr[0]) == 0) {
            mx_start_builtin(l_inf, shell);
            return 1;
        }
    return mx_exec_prog(l_inf);
}

int mx_find_filepath(char **cmd_arr, char **filepath) { // rewrite to builtin 'which'
    char *path = mx_strdup(getenv("PATH"));
    char *token = NULL;
    DIR *dptr;
    struct dirent *ds;

    if (path)
        token = strtok(path, ":");
    for (; token; token = strtok(NULL, ":")) {
        if ((dptr = opendir(token)) != NULL) {
            for (; (ds = readdir(dptr)) != NULL;) {
                if (strcmp(ds->d_name, cmd_arr[0]) == 0) {
                    *filepath = mx_strjoin(token, "/");
                    *filepath = mx_strjoin(*filepath, ds->d_name); // need free in mx_strjoin;
                    return 0; // 2 means == type = binary;
                }
            }
            closedir(dptr);
        }
    }
    return -1;
}
