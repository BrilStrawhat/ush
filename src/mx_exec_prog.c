#include "ush.h"

static void print_error(st_launch *l_inf) {
    if (errno == ENOENT) {
        mx_printerr("ush: command ");
        mx_printerr(l_inf->cmd_arr[0]);
        mx_printerr(" not found\n");
    }
    else {
        mx_printerr("ush: ");
        mx_printstr(strerror(errno));
        mx_printchar('\n');
    }
}

int mx_exec_prog(st_launch *l_inf) { // Not auditor((
    int status = 0;
    pid_t pid;
    char *fp = (l_inf->filepath != NULL) ? l_inf->filepath :l_inf->cmd_arr[0];

    if ((pid = fork()) < 0) {
        print_error(l_inf);
        return -1;
    }
    else if (pid == 0) { // Child process
        if (execve(fp, l_inf->cmd_arr, environ) == -1) { 
            print_error(l_inf);
            return -1;
        }
    }
    else {
        if (wait(&status) < 0) {
            print_error(l_inf);
            return -1;
        }
    }
    return WEXITSTATUS(status);
}
