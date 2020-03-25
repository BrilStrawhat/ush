#include "ush.h"

static void tokensize(char *line, char ***tok, int bufsize, int position) {
    char *token = NULL;

    token = strtok(line, ";");
    while (token != NULL) {
        if (token[0] == '\n' && token[1] == '\0') {
            token = strtok(NULL, ";");
            continue;
        }
        (*tok)[position++] = mx_strtrim(token);
        if (position >= bufsize) {
            bufsize += 64;
            *tok = realloc(tok, bufsize * sizeof(char *));
            if (!(*tok)) {
                fprintf(stderr, "u$h: allocation error\n");
                exit (EXIT_FAILURE);
            }
        }
        token = strtok(NULL, ";");
    }
    (*tok)[position] = NULL;
}


char **mx_tok(char *line) {
    int bufsize = 64;
    int position = 0;
    char **tok = malloc(bufsize * sizeof(char *));

    if (!tok) {
        fprintf(stderr, "u$h: allocation errno\n");
        exit (EXIT_FAILURE);
    }
    tokensize(line, &tok, bufsize, position);
    if (tok[0] == NULL)
        return NULL;
    return tok;
}
