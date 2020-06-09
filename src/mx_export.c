#include "ush.h"

int mx_export(st_launch *l_inf) {
    char **export;
    char *var = NULL;
    char *val = NULL;

    if (mx_get_char_index(l_inf->cmd_arr[1], '=') == -1)
        return 0;
    export = mx_strsplit(l_inf->cmd_arr[1], '=');
    if (export[0] && export[1]){
        var = mx_strdup(export[0]);
        if (export[1])
            val = mx_strdup(export[1]);
        setenv(var, val, 1);
    }
    if (var)
        free(var);
    if(val)
        free(val);
    return 0;
}
