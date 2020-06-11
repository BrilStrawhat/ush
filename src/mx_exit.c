#include "ush.h"

int mx_exit(st_launch *l_inf) {
    int *exit_st = mx_exit_status();

    if (!l_inf->cmd_arr[1]) {
        exit(*exit_st);
    }  
    if (l_inf->cmd_arr[1] && isdigit(atoi(l_inf->cmd_arr[1])))
        exit(atoi(l_inf->cmd_arr[1]));
    
    return 0;
}
