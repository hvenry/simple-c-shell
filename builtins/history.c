#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builtin.h"

// History function
#define HISTORY_SIZE 100

char *history[HISTORY_SIZE];
int history_count = 0;

void add_to_history(const char *line) {
  if (history_count < HISTORY_SIZE) {
    history[history_count++] = strdup(line);
  } else {
    // If history is full, shift commands up and add newline command at the end
    free(history[0]);
    for (int i = 1; i < HISTORY_SIZE; i++) {
      history[i - 1] = history[i];
    }
    history[HISTORY_SIZE - 1] = strdup(line);
  }
}

void free_history() {
  for (int i = 0; i < history_count; i++) {
    free(history[i]);
  }
}

int simple_shell_history(char **args) {
  for (int i = 0; i < history_count; i++) {
    printf("%d %s\n", i + 1, history[i]);
  }
  return 1;
}
