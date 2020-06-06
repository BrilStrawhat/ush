#include"ush.h"

// static void mx_change_dir(char *newdir, t_map **map, int fd) {
//     int result;

//     if (newdir == NULL) {
//         mx_home(map);
//         return;
//     }
//     else if (!strcmp(newdir, "~OLDPWD")) {
//         mx_oldpwd(newdir, map, fd);
//         return;
//     }
//     result = chdir(newdir);
//     if (result < 0) {
//         fprintf(stderr, "cd: %s: ", newdir);
//         perror("");
//     }
//     else
//         mx_change_map(map, newdir);// }

static int parse_flags() {
//     if (!l_inf->cmd_arr[1] || strcmp(l_inf->cmd_arr[1], "--") == 0)
//         return 0;
//     if (l_inf->cmd_arr[1][0] == '-' && l_inf->cmd_arr[1][1]) {
//         if (mx_get_char_index(l_inf->cmd_arr[1], 'P') != -1) {
//             rpath = realpath(getenv("PWD"), NULL);
//             mx_printstr(rpath);
//             if (rpath)
//                 mx_printchar('\n');
//         }
//         if (mx_get_char_index(l_inf->cmd_arr[1], 'L') != -1) {
//             return 0;
//         }
//     }
//     mx_printerr("pwd: bad option\n");

    return 0;
}

static int change_dir(char *path) {
    if (!chdir(path)) {
        setenv("OLDPWD", getenv("PWD"), 1);
        setenv("PWD", path, 1);
        mx_printstr(getcwd(NULL, 0));
        mx_printstr("\n");
        return 0;
    }
    else {
        mx_printerr("cd: no such file or directory: ");
        mx_printerr(path);
        mx_printerr("\n");
        return 1;
    }
}


int mx_cd(st_launch *l_inf) {
    if (!l_inf->cmd_arr[1]){
        if (getenv("HOME"))
            return change_dir(getenv("HOME"));
    }
    else if (strcmp(l_inf->cmd_arr[1], "-") == 0) {
        if (getenv("OLDPWD")) {
            change_dir(getenv("OLDPWD"));
            mx_printstr(getcwd(NULL, 0));
            mx_printstr("\n");
            return 0;
        }
        else if (!getenv("OLDPWD")) {
            mx_printerr("cd: OLDPWD not set\n");
            return 1;
        }
        else if()

    }
    return change_dir(l_inf->cmd_arr[1]);
}
