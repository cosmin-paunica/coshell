extern int shell_cd(char **args);
extern int shell_exit(char **args);

extern int (*builtins[]) (char **);
extern const char * const builtin_strs[];
extern const int num_builtins;
