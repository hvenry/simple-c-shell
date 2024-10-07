#include <stdio.h>
#include "builtin.h"

// List of built-in commands, followed by their corresponding functions.
char *builtin_str[] = {
  "cd",
  "help",
  "exit",
  "history"
};

// Array of function pointers that take array of strings and return int
int (*builtin_func[]) (char **) = {
  &simple_shell_cd,
  &simple_shell_help,
  &simple_shell_exit,
  &simple_shell_history
};

// Our number of builtins
int simple_shell_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

// help function
int simple_shell_help(char **args) {
  printf("\n");
  printf(
    "   _  _ __   __ ___  _  _  ___ __   __       ___  ___  __  __  ___  _     ___        ___  _  _  ___  _     _    \n"
    "  | || |\\ \\ / /| __|| \\| || _ \\\\ \\ / /      / __||_ _||  \\/  || _ \\| |   | __|      / __|| || || __|| |   | |   \n"
    "  | __ | \\   / | _| | .  ||   / \\   /       \\__ \\ | | | |\\/| ||  _/| |__ | _|       \\__ \\| __ || _| | |__ | |__ \n"
    "  |_||_|  \\_/  |___||_|\\_||_|_\\  |_|        |___/|___||_|  |_||_|  |____||___|      |___/|_||_||___||____||____|\n"
  );
  printf("\n");


  printf("Forked from Stephen Brennan's lsh, this is a small project used to understand how a shell works. It explores how a\n"
         "typical shell initailzes, interprets, and terminates, and implements the foundational loop of reading, parsing and\n"
         "executing commands.\n\n\n");
  printf("The following is a list of built in commands:\n");

  for (int i = 0; i < simple_shell_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("\nUse the man command for information on other programs.\n");
  return 1;
}

