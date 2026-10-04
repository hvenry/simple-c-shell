# Builtins

Commands the shell runs in its own process instead of forking: `cd`, `help`, `exit`, and `history`.

## Why

Some commands must change the shell's own state.
A forked `cd` would change the child's directory and vanish when it exits, and `exit` has to stop the parent loop.
`history` needs the shell's in-memory record, which a child process cannot see.

## How it works

- `builtins/help.c` owns the dispatch table: `builtin_str[]` (names) and `builtin_func[]` (function pointers), index-aligned.
- `simple_shell_num_builtins` derives the count from `sizeof(builtin_str)`, so it updates automatically.
- `simple_shell_execute` in `simple_shell.c` compares `args[0]` against each name with `strcmp` and calls the matching function.
- Every builtin has the signature `int (char **args)` and returns 1 to keep the loop running or 0 to exit.

| Builtin | Behaviour |
|---|---|
| `cd <dir>` | `chdir(args[1])`; errors if no argument (no `$HOME` fallback). |
| `help` | Prints a banner, a description, and the builtin names from the table. |
| `exit` | Returns 0; ignores arguments, so there is no exit code. |
| `history` | Prints numbered entries from the in-memory buffer. |

**History.** `builtins/history.c` keeps up to 100 `strdup`'d lines in a fixed array.
When full, it frees the oldest entry and shifts the rest down by one (O(n) per insert).
The loop calls `add_to_history` for every line before parsing.

**Adding a builtin:**

1. Declare `int simple_shell_<name>(char **args);` in `builtins/builtin.h`.
2. Implement it in `builtins/<name>.c`.
3. Add the name to `builtin_str` and the function to `builtin_func` at the same index in `builtins/help.c`.
4. Add a `builtins/<name>.o` rule and append it to the `simple_shell` link line in `Makefile`.

## Key files

- `builtins/builtin.h` - declarations and `extern` dispatch table.
- `builtins/help.c` - dispatch table, count, and `help`.
- `builtins/cd.c`, `builtins/exit.c`, `builtins/history.c` - one builtin each.

## Decisions and gotchas

- The dispatch table lives in `help.c` because `help` is its main reader; it is the one place to register a builtin.
- History is session-only; nothing is written to disk.
- `free_history` exists but is never called, so history leaks on exit (harmless at process exit, noisy under a leak checker).
- History numbering restarts at 1 for the oldest kept entry once the buffer wraps.

## Related

- [Command loop](command-loop.md)
