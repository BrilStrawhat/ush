#include "ush.h"

bool mx_if_exec(char *filepath) {
    struct stat sb;

    if (stat(filepath, &sb) == 0 && sb.st_mode & S_IXUSR) 
        return true;
    else
        return false;
}

static void flag_parser(char **argv, bool *flags, int *i) {
    regex_t preg;
    
    regcomp(&preg, "^-[as]+$", REG_EXTENDED);
    for (; argv[*i]; (*i)++) {
        if (0 == (regexec(&preg, argv[*i], 0, NULL, 0))) {
            for (int j = 1; argv[*i][j] != '\0'; j++) {
                if (argv[*i][j] == 'a')
                  flags[0] = true; 
                else if(argv[*i][j] == 's') {
                  flags[1] = true; 
                  flags[0] = false;
                  break;
                }
            }
        }
        else 
            break;
    }
    regfree(&preg);
}

int mx_which(st_launch *l_inf) {
    bool flags[2] = {false}; // -a = 0, -s = 1;
    int i = 1;
    char *filepath = NULL;
    
    flag_parser(l_inf->cmd_arr, flags, &i);
    for (; l_inf->cmd_arr[i]; i++) {
        if (mx_find_filepath(&l_inf->cmd_arr[i], &filepath, flags) != 1) {
            if (flags[1] == true) {
                if (mx_if_exec(filepath) == true)
                    return 0;
            }
            else {
                if (mx_if_exec(filepath) == true) {
                    mx_printstr(filepath);
                    mx_printchar('\n');
                    return 0;
                }
            }
        }
    }
    return 0;
}
