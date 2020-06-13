#include "ush.h"

static void costil_2(DIR *dptr, char **path) {
    closedir(dptr);
    mx_strdel(path);
}


static int costil_auditor(char **fpath, bool *flags, DIR *dptr, char **path) {
    if (flags == NULL) {
        costil_2(dptr, path);
        return 0;
    }
    if (flags[0] == true) {
        if (mx_if_exec(*fpath) == true) {
            mx_printstr(*fpath);
            mx_printchar('\n');
            mx_strdel(fpath);
        }
        return 1;
    }
    else {
        costil_2(dptr, path);
        return 0;
    }
}

int mx_find_filepath(char **cmd_arr, char **filepath, void *flags) { 
    char *path = getenv("PATH");
    DIR *dptr;

    if (path == NULL)
        path = mx_find_filepath2();
    if (path != NULL) {
        path = mx_strdup(path);
        for (char *tok = strtok(path, ":"); tok; tok = strtok(NULL, ":")) {
            if ((dptr = opendir(tok)) != NULL) {
                for (struct dirent *ds; (ds = readdir(dptr)) != NULL;) {
                    if (strcmp(ds->d_name, cmd_arr[0]) == 0) {
                        *filepath = mx_three_to_one(tok, "/", ds->d_name);
                        if (costil_auditor(filepath, flags, dptr, &path) == 0)
                            return 1;
                    }
                }
                closedir(dptr);
            }
        }
    }
    mx_strdel(&path);
    return 1;
}
