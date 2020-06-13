#include "ush.h"

static char *read_line(void) {
    char *tmp = NULL;
    size_t bufsize = 0;

    getline(&tmp, &bufsize, stdin);
    return tmp;
}

static void signal_for_parent(void) {
    signal(SIGINT, SIG_IGN); // C-c
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
}

int mx_check_jobs(void) {
    t_list **jobs = mx_jobs_list();

    if (jobs == NULL || *jobs == NULL)
        return 0;
        t_list *cur = *jobs;
        t_list *bef = NULL;

    if (kill(((t_job*)((cur)->data))->pid, 0) != 0) {
        cur = (cur)->next; 
        mx_free_job((t_job*)cur->data);
        free(cur);
    }
    while (cur != NULL) {
        if (kill(((t_job*)((cur)->data))->pid, 0) == true) {
            bef->next = cur->next;
            mx_free_job((t_job*)cur->data);
            free(cur);
            mx_check_jobs();
        }
        bef = cur;
        cur = cur->next;
    }
    return 1;
    
}
void mx_loop(t_head *head) {
    char *line = NULL;
    int status = 0;
    t_shell *shell = (t_shell*)malloc(sizeof(t_shell));
    bzero(shell, sizeof(t_shell));

    mx_init_shell(shell);
    signal_for_parent();
    while (1) {
        // mx_check_jobs();
        mx_printstr("u$h> ");
        line = read_line();
        if (!mx_strtrim(line))// new
            continue;// new
        line[strlen(line)-1] = '\0';
        if (line) {
            if (mx_check_line(line) == 1) {
                mx_printstr("syntax error\n");
                status = 258;// errno
                continue;
            }
        // line = valid ${}
            /*line = mx_ex_param(line);
            if (!line) {
                mx_printerr("error param {}\n");// after error
                continue;
            }
        */
           //mx_tilda(&line);
           //mx_printstr(line);
        
            head = mx_create_head(line);
            
            //mx_printforest(head);
            if (!head) { // new
                continue;
            }
            if (line != NULL) {
                mx_launch_cmd(head, shell);
                continue;
            }

        }
    }
}
