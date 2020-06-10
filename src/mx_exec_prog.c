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
    t_job *job = (t_job*)malloc(sizeof(t_job));
    char *cmd = NULL;
    int len = 0;
    int i;

    for (i = 0; l_inf->cmd_arr[i]; i++)
        len += mx_strlen(l_inf->cmd_arr[i]);
    cmd = mx_strnew(len + i);
    for (i = 0, len = 0; l_inf->cmd_arr[i]; i++) {
        for (int j = 0; l_inf->cmd_arr[i][j]; j++) {
            cmd[len++] = l_inf->cmd_arr[i][j];
        }
        cmd[len++] = ' ';
    }
    job->cmd = cmd;
    job->pid = pid;
    job->status = status;
    return job;
}

void mx_add_to_list(st_launch *l_inf, pid_t pid, t_list **jobs, int status) {
    if (*jobs == NULL)
        mx_printstr("tut\n");
    if (*jobs == NULL)
        *jobs = (t_list*)malloc(sizeof(t_list)); // mx_create_node???
    t_job *job = mx_create_job(l_inf, pid, status);
    if (job == NULL || jobs == NULL || *jobs == NULL)
       mx_printstr("\n\n\n\n\n\n\tut\n\n\n\n\n\n\n\n");
    if (job && jobs)
       mx_printstr("Hello");
    mx_printstr(job->cmd);
    mx_printchar(' ');
    mx_printint(job->pid);
    mx_printchar(' ');
    mx_printint(job->status);
    mx_printchar('\n');
    (*jobs)->data = job;
    (*jobs)->next = NULL;
   
    // mx_push_back(jobs, job);
}

void signal_for_child(void) {
    signal(SIGINT, SIG_DFL); // C-c
    signal(SIGQUIT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
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
