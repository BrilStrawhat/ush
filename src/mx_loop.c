#include "ush.h"

static char *read_line(void) {
    char *tmp = NULL;
    size_t bufsize = 0;

    getline(&tmp, &bufsize, stdin);
    return tmp;
}

void mx_loop(t_head *head) {
    char *line = NULL;
    int status = 0;
    
    while (1) {
        mx_printstr("u$h> ");
        line = read_line();
        if (line) {
            if (mx_check_line(line) == 1) {
                mx_printstr("error\n");
                status = 258;// errno
                continue;
            }
        // line = valid ${}
            head = mx_create_head(line);
            
            if (head) {
                mx_printstr("HELLo");
            }
        }
    }
}
