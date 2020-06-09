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

// int mx_fg(st_launch *l_inf, t_list *jobs, pid_t *pid) {
    
// }


t_job *mx_create_job(st_launch *l_inf, pid_t pid, int status) {
    t_job *job = (t_job*)malloc(sizeof(job));
    char *cmd = (l_inf->filepath != NULL) ? l_inf->filepath : l_inf->cmd_arr[0];
    char *tmp = NULL;

    if (l_inf->cmd_arr[1] != NULL) {
        cmd = mx_three_to_one(cmd, " ", l_inf->cmd_arr[1]);
        for (int i = 2; l_inf->cmd_arr[1] && l_inf->cmd_arr[i]; i++) {
            tmp = mx_three_to_one(cmd, " ", l_inf->cmd_arr[i]);
            mx_strdel(&cmd);
            cmd = tmp;
            mx_strdel(&tmp);
        }
        job->cmd = cmd;
        mx_strdel(&cmd);
    }
    else
        job->cmd = mx_strdup(cmd);
    job->pid = pid;
    job->status = status;
    return job;
}

void mx_add_to_list(st_launch *l_inf, pid_t pid, t_list **jobs, int status) {
   if (*jobs == NULL)
        *jobs = (t_list*)malloc(sizeof(t_list));
   t_job *job = mx_create_job(l_inf, pid, status);
   mx_push_back(jobs, job);
}

void signal_for_child(void) {
    signal(SIGINT, SIG_DFL); // C-c
    signal(SIGQUIT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    signal(SIGCHLD, SIG_DFL);
}

int mx_exec_prog(st_launch *l_inf, t_list **jobs) { // Not auditor((
    int status = 0;
    pid_t pid;
    char *fp = (l_inf->filepath != NULL) ? l_inf->filepath : l_inf->cmd_arr[0];

    if ((pid = fork()) < 0) {
        print_error(l_inf);
        return -1;
    }
    else if (pid == 0) { // Child process
        signal_for_child();
        if (execve(fp, l_inf->cmd_arr, environ) == -1) { 
            print_error(l_inf);
            return -1;
        }
    }
    else {
        waitpid(pid, &status, WUNTRACED);
        if (WIFSTOPPED(status) == true)
            mx_add_to_list(l_inf, pid, jobs, status);
    }
    return status;
}
