#include <stdio.h>
#include <unistd.h>
#include "builtin.h"

// cd function
int simple_shell_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("cd");
        }
    }
    return 1;
}

