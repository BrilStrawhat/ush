#include "ush.h"

int mx_unset(st_launch *l_inf) {
    if (l_inf->cmd_arr[1]){
        unsetenv(l_inf->cmd_arr[1]);
    }
    return 0;
}
