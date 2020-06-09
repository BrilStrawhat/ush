#include "ush.h"


int main(int args, char* argv[]) {
    t_head head;

    // signal (SIGINT, SIG_IGN); // C-c
    signal (SIGQUIT, SIG_IGN);
    signal (SIGTSTP, SIG_IGN);
    signal (SIGTTIN, SIG_IGN);
    signal (SIGTTOU, SIG_IGN);
    signal (SIGCHLD, SIG_IGN);
    memset(&head, 0, sizeof(t_head));
    if (args && argv) {
       mx_loop(&head);
    }
}
