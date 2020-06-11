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

int mx_jobs(t_list **jobs) {
   if (jobs == NULL || *jobs == NULL)
      return 0;
    t_list *tj = *jobs;
    t_job *cast = NULL;
 
    for (; tj != NULL; tj = tj->next) {
        cast = (t_job*)tj->data;         
        mx_printint(cast->pid);
        mx_printstr(" ");
        mx_printstr(cast->cmd);
        mx_printstr("\n");
    }
    return 0;
}

int mx_pop_job(t_list **jobs, int num) {
    if (jobs == NULL || *jobs == NULL)
        return 0;
    t_list *cur = *jobs;
    t_list *bef = NULL;

    for (int i = 0; cur != NULL || i < num; i++)


}

int mx_fg(st_launch *l_inf, t_list **jobs, pid_t *pid) {
    if (jobs == NULL || *jobs == NULL)
        return 0;
    mx_pop_job(jobs, atoi(l_inf->cmd_arr[1]));
    return 0;
}


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
    t_job *job = mx_create_job(l_inf, pid, status);

    mx_push_back(jobs, job);
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
