#include "ush.h"

static void costil(char **filepath) {
    mx_printstr(*filepath);
    mx_strdel(*filepath);
}

static void flag_parser(int argc, char **argv, bool *flags, int *i) {
    regex_t preg;
    
    regcomp(&preg, "^-[as]+$", REG_EXTENDED);
    for (; *i < argc; (*i)++) {
        if (0 == (regexec(&preg, argv[*i], 0, NULL, 0))) {
            for (int j = 1; argv[*i][j] != '\0'; j++) {
                if (argv[*i][j] == 'a')
                  flags[0] = true; 
                else if(argv[*i][j] == 's')
                  flags[1] = true; 
            } 
        else 
            break;
    }
    regfree(&preg);
}

static int mx_find_filepath(char **cmd_arr, char **filepath) { 
    char *path = getenv("PATH");
    DIR *dptr;
    struct dirent *ds;

    if (path != NULL) {
        for (char *tok = strtok(path, ":"); tok; tok = strtok(NULL, ":")) { // need to free tok??
            if ((dptr = opendir(tok)) != NULL) {
                for (; (ds = readdir(dptr)) != NULL;) {
                    if (strcmp(ds->d_name, cmd_arr[0]) == 0) {
                        *filepath = mx_three_to_one(tok, "/", ds->d_name);
                        costil(filepath);
                    }
                }
                closedir(dptr);
            }
        }
    }
    return -1;
}

int mx_which(st_launch *l_inf) {
    
}
