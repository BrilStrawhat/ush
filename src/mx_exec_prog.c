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

int mx_free_job(t_job *job) {
    if (job == NULL) {
        return 1;
    }
    mx_strdel(&job->cmd);
    free(job);
    return 0;
}

int  mx_pop_front_job(t_list **jobs) {
    t_list *cur = *jobs;

    kill(((t_job*)((*jobs)->data))->pid, SIGCONT);
    *jobs = (*jobs)->next; 
    mx_free_job((t_job*)cur->data);
    free(cur);
    return 0;
}

int mx_pop_job(t_list **jobs, int num) {
    if (jobs == NULL || *jobs == NULL)
        return 0;
    t_list *cur = *jobs;
    t_list *bef = NULL;

    if (num == 1)
        return mx_pop_front_job(jobs);
    for (int i = 1; cur != NULL && i <= num; i++) {
        if (i == num) {
            kill(((t_job*)((*jobs)->data))->pid, SIGCONT);
            bef->next = cur->next;
            mx_free_job((t_job*)cur->data);
            free(cur);
            return 0;
        }
        bef = cur;
        cur = cur->next;
    }
    return 1;
}

int mx_cont_job(int num) {
    t_list **cur = mx_jobs_list();
    int status = 0;

    for (int i = 1; *cur != NULL && i <= num; i++) {
        if (i == num) {
            kill(((t_job*)((*cur)->data))->pid, SIGCONT);
            waitpid(((t_job*)((*cur)->data))->pid, &status, WUNTRACED);
            if (WIFSTOPPED(status) == true)
                return 0;
            else if (WIFEXITED(status) == true) {
                mx_pop_job(cur, i);
                return 0;
            }
        }
        *cur = (*cur)->next;
    }
    return 1;
}

int mx_fg(st_launch *l_inf, t_list **jobs) {
    if (jobs == NULL || *jobs == NULL)
        return 0;
    if (l_inf->cmd_arr[1] == NULL) {
        return mx_cont_job(1);
    }
    
    return mx_cont_job(atoi(l_inf->cmd_arr[1]));
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

void shti(void) {
    signal(SIGINT, SIG_IGN); // C-c
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
}

void mx_set_ctrl_term(pid_t pid) {
    tcsetpgrp(0, pid);
    tcsetpgrp(1, pid);
    tcsetpgrp(2, pid);
}

int mx_exec_prog(st_launch *l_inf, t_list **jobs) { // Not auditor((
    int status = 0;
    pid_t pid;
    char *fp = (l_inf->filepath != NULL) ? l_inf->filepath : l_inf->cmd_arr[0];
    pid_t ppid = getpid();

    if ((pid = fork()) < 0) {
        print_error(l_inf);
        return -1;
    }
    else if (pid == 0) { // Child process
        signal_for_child();
        mx_set_ctrl_term(getpid());
        setpgid(getpid(), getpid());
        if (execve(fp, l_inf->cmd_arr, environ) == -1) { 
            print_error(l_inf);
            exit(-1);
        }
    }
    else {
        if (waitpid(pid, &status, 0) < 0) {
            mx_printstr(strerror(errno));
            mx_printstr("\n");
        }
        // shti();
        mx_set_ctrl_term(ppid);
        if (WIFSTOPPED(status) == true)
            mx_add_to_list(l_inf, pid, jobs, status);
    }
    return status;
}
