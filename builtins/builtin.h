#ifndef BUILTIN_H
#define BUILTIN_H

// Declarations for built-in commands
// Function Declarations for built-in shell commands:
// Fowward declaration: something is declared but not defiend, we do this to break the
// dependancy cycle of simple_shell_help() using array of builtinss containing simple_shell_help()
int simple_shell_cd(char **args);
int simple_shell_help(char **args);
int simple_shell_exit(char **args);
int simple_shell_history(char **args);

// Function to keep track of command history
void add_to_history(const char *line);
void free_history();

// Built-in command array
extern char *builtin_str[];

// array of function pointers for built-in commands
extern int (*builtin_func[])(char **);

// get number of bultins
int simple_shell_num_builtins(void);

#endif
