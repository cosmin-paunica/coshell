#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int shell_cd(char **args)
{
    char *dir = args[1];
    if (dir == NULL) {
        dir = "~";
    }

    if (chdir(dir) != 0) {
        perror("lsh");
    }

    return 1;
}

int shell_exit(char **args)
{
    return 0;
}

int (*builtins[]) (char **) = {
    &shell_cd,
    &shell_exit,
};

const char * const builtin_strs[] = {
    "cd",
    "exit",
};

const int num_builtins = sizeof(builtin_strs) / sizeof (char *);
