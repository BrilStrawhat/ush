#include "ush.h"

// static void mx_launch_menu(char *cmd, t_shell *shell) { // We do not use this function!
     // st_launch *l_inf = mx_launch_init(cmd, shell); // create cmd_arr // create param_arr // find type // find filepath // exp param
     // if (l_inf) {
         // if (l_inf->type == 1)
             // mx_start_builtin(l_inf, shell);
         // else if (l_inf->type == 2)
            // mx_exec_prog(l_inf, shell);
         // }
         // else
             // shell->status = 127; // echo $?;
// }

void mx_launch_cmd(t_head *forest, t_shell *shell) {
    while(forest) {
        while (forest->command) {
            if (forest->command->cmd)
                mx_launch_init(forest->command->cmd, shell); // LAUNCHING CMD
            if (forest->command->or != NULL && shell->status != 0) {
                forest->command = forest->command->or;
               continue;
            }
            else if (shell->status == 0) {
                if (forest->command->and && !forest->command->or)
                    forest->command = forest->command->and;
                else if (!forest->command->and && forest->command->or) {
                    while (forest->command && !forest->command->and)
                        forest->command = forest->command->or;
                    if (forest->command)
                        forest->command = forest->command->and;
                }
                else if (!forest->command->and && !forest->command->or)
                    forest->command = NULL;
                continue;
            }
            else
                break;
        }
        forest = forest->next;
    }
}
