#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIMPLE_SHELL_RL_BUFSIZE 1024
#define SIMPLE_SHELL_TOK_BUFSIZE 64
#define SIMPLE_SHELL_TOK_DELIM " \t\r\n\a"

// Function Declarations for built-in shell commands:
// Fowward declaration: something is declared but not defiend, we do this to break the
// dependancy cycle of simple_shell_help() using array of builtinss containing simple_shell_help()
int simple_shell_cd(char **args);
int simple_shell_help(char **args);
int simple_shell_exit(char **args);

// List of built-in commands, followed by their corresponding functions.
char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

// Array of function pointers that take array of strings and return int
int (*builtin_func[]) (char **) = {
  &simple_shell_cd,
  &simple_shell_help,
  &simple_shell_exit
};

// Our number of builtins
int simple_shell_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/* Built-in function implementations */

// cd function
int simple_shell_cd(char **args) {
  // cd first checks if second argument exists
  if (args[1] == NULL) {
    fprintf(stderr, "simple_shell: expected argument to \"cd\"\n");
  }
  // call chdr() and check for errors
  else {
    if (chdir(args[1]) != 0) {
      perror("simple_shell");
    }
  }
  return 1;
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


  printf("Forked From: Stephen Brennan's lsh\n\n\n");
  printf("Type program names and arguments, and hit enter.\n\n");
  printf("The following are built in:\n");

  for (int i = 0; i < simple_shell_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("\nUse the man command for information on other programs.\n");
  return 1;
}

// Exit function that returns 0, which 
int simple_shell_exit(char **args) {
  return 0;
}

// Function to launch a program with the list of tokens
int simple_shell_launch(char **args) {
  pid_t pid, wpid;
  int status;

  // Fork our process
  pid = fork();

  // Child process (where pid == 0)
  if (pid == 0) {
    // Run the command given by the user using exec system call execvp
    // This expects a program name and vector
    if (execvp(args[0], args) == -1) {
      // -1 signals an error, so we use perror to print the systems error message
      perror("simple_shell");
    }
    exit(EXIT_FAILURE);
  // Check if fork() had an error
  } else if (pid < 0) {
    perror("simple_shell");
  } 
  // Parent process, fork() executed sucessfully
  else { 
    do {
      // parent process needs wait for childs command to finish running
      // waitpid() waits for the process's state to change to either exited or killed
      wpid = waitpid(pid, &status, WUNTRACED);
    // do this until WIFEXITED: process exited, or WIFSIGNALED: process terminated
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  
  // return 1 to signal that we should propmt for input again
  return 1;
}

// Function to execute shell commands
int simple_shell_execute(char **args) {
  // check if args is NULL, ie if user enters whitespace or an empty string
  if (args[0] == NULL) {
    return 1;
  }

  for (int i = 0; i < simple_shell_num_builtins(); i++) {
    // Check if command equals each builtin, if so, run it
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  // If the current arg does not match a builtin, use simple_shell_launch() to launch a process
  return simple_shell_launch(args);
}

// Function to read a line of input (alternative implementation can use getline())
char *simple_shell_read_line(void) {
  int bufsize = SIMPLE_SHELL_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "simple_shell: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character, store as int because EOF is an int (-1)
    c = getchar();

    // If we hit EOF or newline, replace with null terminator and return
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    }
    // If not the EOF, add character to existing string
    else {
      buffer[position] = c;
    }
    position++;

    // If we exceed the buffer, reallocate
    // We d this by comparing the incremented position to the buffersize
    if (position >= bufsize) {
      bufsize += SIMPLE_SHELL_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "simple_shell: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

// Function to split a line into tokens (arguments)
char **simple_shell_split_line(char *line) {
  int bufsize = SIMPLE_SHELL_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "simple_shell: allocation error\n");
    exit(EXIT_FAILURE);
  }

  // strtok() splits our string into a sequence of tokens based on the delimiter defined
  token = strtok(line, SIMPLE_SHELL_TOK_DELIM);

  // parse each token, move position to next spot in array
  while (token != NULL) {
    tokens[position] = token;
    position++;

    // dynamically increase buffer size of bufsize
    if (position >= bufsize) {
      bufsize += SIMPLE_SHELL_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "simple_shell: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
    
    // move to the next token
    token = strtok(NULL, SIMPLE_SHELL_TOK_DELIM);
  }
  tokens[position] = NULL;

  // return tokens
  return tokens;
}

// Main loop function: Read, Parse, Execute
void simple_shell_loop(void) {
  char *line;
  char **args;
  int status;

  do {
    // print a prompt
    printf("> ");
    // Read: Read in a line
    line = simple_shell_read_line();
    // Parse: Split line into arguments
    args = simple_shell_split_line(line);
    // Execute: Execute the args, and set status to see if we should exit or not
    status = simple_shell_execute(args);

    // Free line and arguments after (this deallocates memory dynamically)
    free(line);
    free(args);
  } while (status);
}

// Main entry point: Initialze, Interpret, Terminate
int main(int argc, char **argv) {
  // 1. Initialize: Load config files, if any.

  // 2. Interpret: Run command loop.
  simple_shell_loop();

  // 3. Terminate: Perform any shutdown/cleanup.
  return EXIT_SUCCESS;
}

