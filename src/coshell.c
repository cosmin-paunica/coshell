#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define LINE_BUFFER_SIZE 1024
#define ARGS_BUFFER_SIZE 64
#define ARGS_DELIM " \t\r\n\a"

char *read_line()
{
    int buffer_size = LINE_BUFFER_SIZE;
    char *buffer = malloc(buffer_size * sizeof(char));

    if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    int position = 0;
    int c;

    while (1) {
        c = getchar();

        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        }
        
        buffer[position++] = c;

        if (position >= buffer_size) {
            buffer_size *= 2;
            buffer = realloc(buffer, buffer_size * sizeof(char));

            if (!buffer) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **parse_line(char *line)
{
    int buffer_size = ARGS_BUFFER_SIZE;
    char **args = malloc(buffer_size * sizeof(char *));

    if (!args) {
        fprintf(stderr, "lsh: allocation error");
        exit(EXIT_FAILURE);
    }

    int position = 0;
    
    char *arg = strtok(line, ARGS_DELIM);
    while (arg != NULL) {
        args[position++] = arg;

        if (position >= buffer_size) {
            buffer_size *= 2;
            args = realloc(args, buffer_size * sizeof(char *));

            if (!args) {
                fprintf(stderr, "lsh: allocation error");
                exit(EXIT_FAILURE);
            }
        }

        arg = strtok(NULL, ARGS_DELIM);
    }

    args[position] = NULL;
    return args;
}

int launch(char **args)
{
    pid_t pid = fork(), wpid;
    int status;

    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("coshell");
        }

        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("coshell");
    } else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int execute(char **args)
{
    if (args[0] == NULL) {
        return 1;
    }

    int i;
    for (i = 0; i < num_builtins; i++) {
        if (strcmp(args[0], builtin_strs[i]) == 0) {
            return (*builtins[i])(args);
        }
    }

    return launch(args);
}

void loop()
{
    char *line;
    char **args;
    int status;

    do {
        printf("%s", "> ");

        line = read_line();
        args = parse_line(line);

        status = execute(args);

        free(line);
        free(args);
    } while (status);
}

int main(int argc, char *argv[])
{
    loop();

    return EXIT_SUCCESS;
}
