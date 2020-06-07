#include"ush.h"


static int flag_s(st_launch *l_inf) {
    struct stat st;
    char *pwd = mx_strdup(getenv("PWD"));
    chdir(l_inf->cmd_arr[2]);
    if (lstat(getcwd(NULL, 0), &st) != -1 && MX_IS_LNK(st.st_mode))
        printf("Hello\n");
       printf("%d", MX_IS_LNK(st.st_mode));
    free(pwd);
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
        else {
            mx_printerr("cd: OLDPWD not set\n");
            return 1;
        }
    }
    else if (strcmp(l_inf->cmd_arr[1], "-s") == 0) {
        return flag_s(l_inf);
    }
    return change_dir(l_inf->cmd_arr[1]);
}
