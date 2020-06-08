#include"ush.h"

static char *path_constructor(char *path, char *fname) {
    int len = mx_strlen(fname);
    char *path_with_slash = mx_strjoin(path, "/");
    char tmp[len];
    char *res = NULL;

    if (fname[len - 1] != '/')
        res = mx_strjoin(path_with_slash, fname);
    else {
        mx_strncpy(tmp, fname, len - 1);
        tmp[len - 1] = '\0';
        res = mx_strjoin(path_with_slash, tmp);
    }

    mx_strdel(&path_with_slash);
    return res;
}

int mx_strarr_len(char **strarr) {
    int len = 0;

    if (!strarr)
        return len;
    while (strarr[len])
        len++;
    return len;
}

char *mx_build_path(char *pwd, char *fname) {
    char *full_path = NULL;

    if (fname[0] == '/')
        full_path = mx_strdup(fname);
    else
        full_path = path_constructor(pwd, fname);

    return full_path;
}


char *mx_strarr_to_str(char **strarr, char *delim) {
    char *str = NULL;
    char *tmp = NULL;

    for (int i = 0; strarr && strarr[i]; i++) {
        tmp = mx_strjoin(str, strarr[i]);
        mx_strdel(&str);
        if (strarr[i + 1]) {
            str = mx_strjoin(tmp, delim);
            mx_strdel(&tmp);
        }
        else
            str = tmp;
    }

    return str;
}

char mx_check_link(char **path, char *full_path) {
    struct stat st;

    mx_strdel(path);
    if (lstat(full_path, &st) != -1 && MX_IS_LNK(st.st_mode))
        return 1;

    return 0;
}

static char  *includes_link_P(char *destination) {
    char **split = mx_strsplit(destination, '/');
    char *path = NULL;
    char *full_path = NULL;

    while (split[0]) {
        path = mx_strarr_to_str(split, "/");
        if (destination[0] == '/')
            full_path = mx_strjoin("/", path);
        else
            full_path = mx_build_path(getenv("PWD"), path);
        if (mx_check_link(&path, full_path)) {
            mx_del_strarr(&split);
            return full_path;
        }
        mx_strdel(&split[mx_strarr_len(split) - 1]);
        mx_strdel(&full_path);
    }
    free(split);
    return NULL;
}

static char includes_link(char *destination) {
    char **split = mx_strsplit(destination, '/');
    char *path = NULL;
    char *full_path = NULL;

    while (split[0]) {
        path = mx_strarr_to_str(split, "/");
        if (destination[0] == '/')
            full_path = mx_strjoin("/", path);
        else
            full_path = mx_build_path(getenv("PWD"), path);
        if (mx_check_link(&path, full_path)) {
            mx_del_strarr(&split);
            mx_strdel(&full_path);
            return 1;
        }
        mx_strdel(&split[mx_strarr_len(split) - 1]);
        mx_strdel(&full_path);
    }
    free(split);
    return 0;
}


static int change_dir(st_launch *l_inf, char *path) {
    char *link = NULL;
    if (!chdir(path)) {
        if (strcmp(getenv("OLDPWD"), getenv("PWD")) != 0)
            setenv("OLDPWD", getenv("PWD"), 1);
        setenv("PWD", getcwd(NULL, 0), 1);
        if (includes_link(path)){
            link = mx_strdup(includes_link_P(path));
            setenv("PWD", link, 1);
            free(link);
        }
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
            return change_dir(l_inf, getenv("HOME"));
    }
    else if (strcmp(l_inf->cmd_arr[1], "-") == 0) {
        if (getenv("OLDPWD")) {
            mx_printstr(getenv("OLDPWD"));
            mx_printchar('\n');
            return change_dir(l_inf, getenv("OLDPWD"));
    }   
        else {
            mx_printerr("cd: OLDPWD not set\n");
            return 1;
        }
    }
    else if (strcmp(l_inf->cmd_arr[1], "-s") == 0 && includes_link(l_inf->cmd_arr[2])){
        // mx_printstr(includes_link(l_inf->cmd_arr[2]));
        mx_printerr("cd: not a directory\n"); 
    }
    else if (l_inf->cmd_arr[1] && strcmp(l_inf->cmd_arr[1], "-P") ==0)
        return change_dir(l_inf, l_inf->cmd_arr[2]);
    return change_dir(l_inf, l_inf->cmd_arr[1]);
}
