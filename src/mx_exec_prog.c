#include "ush.h"

// extern char **environ;

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

int mx_exec_prog(st_launch *l_inf) {
    int status = 0;
    pid_t pid;

    if ((pid = fork()) < 0) {
        print_error(l_inf);
        return -1;
    }
    else if (pid == 0) { // Child process
        if (execvp(l_inf->cmd_arr[0], l_inf->cmd_arr) == -1) { // need to change to execve
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
    return status;
}
