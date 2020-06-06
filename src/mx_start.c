#include "ush.h"

int  mx_start_builtin(st_launch *l_inf, t_shell *shell) {
     for (int i = 0; shell->builtins[i]; i++) {
        if (strcmp(l_inf->cmd_arr[0], shell->builtins[i]) == 0) { // find biultin in finction (mx_init_shell)    // add your builtins
           // shell->status = your_function(l_inf->cmd_arr, shell); // your function where you work with builtins/   you need return shell->statur = 0; if Ok!
           if (strcmp("env", shell->builtins[i]) == 0) {
               return mx_env(l_inf, shell);
               
           }
           else if (strcmp("echo", shell->builtins[i]) == 0){
               return mx_echo(l_inf->cmd_arr, shell);
           }
               mx_printstr("Builtins\n");
        }
    }
    return 0;
}
