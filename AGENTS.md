# Simple C Shell

A minimal Unix shell in C, built to learn how a shell reads, parses, and executes commands.
It runs a read-parse-execute loop, dispatches a few builtins in-process, and forks/execs everything else from `PATH`.
Based on Stephen Brennan's `lsh` tutorial.
Stack: C, POSIX (`fork`, `execvp`, `waitpid`), GNU Make, GCC or Clang.

## Commands
```bash
make                                       # build ./simple_shell
./simple_shell                             # start the interactive shell
printf 'echo hi\nexit\n' | ./simple_shell  # non-interactive smoke run; always end input with exit (EOF loops forever)
make clean                                 # remove build outputs (currently broken: Makefile typo `rm -f*.o`)
```
No test suite or linter yet; `docs/specs/test-suite.md` adds both.
Until then, the smoke run above is the only check.

## Repo map
```
simple_shell.c        main, command loop, line reader, tokenizer, fork/exec launcher
builtins/builtin.h    builtin declarations and the shared dispatch table externs
builtins/help.c       dispatch table (builtin_str, builtin_func) and the help builtin
builtins/cd.c         cd builtin
builtins/exit.c       exit builtin
builtins/history.c    in-memory history buffer and the history builtin
Makefile              explicit per-object build rules
screenshots/          README image
```

## Conventions
- **Builtins return 1 to continue, 0 to exit.** The loop in `simple_shell_loop` stops on 0; returning anything else from a builtin silently changes control flow.
- **`builtin_str` and `builtin_func` stay index-aligned.** Dispatch matches a name by index and calls the function at the same index; a mismatch runs the wrong builtin.
- **Anything that changes shell state is a builtin, never a forked command.** `chdir` or similar in a child process is lost when the child exits.
- **Every new `.c` file gets its own rule and an entry in the `simple_shell` link line in `Makefile`.** Objects are listed explicitly; a missing entry fails at link time.
- **Prefix public functions with `simple_shell_`.** C has no namespaces; the prefix avoids clashes with libc and system headers.
- **Report errors to stderr with `perror` or `fprintf(stderr, ...)`.** stdout carries command output, and mixing the two breaks piping the shell's output.

## Docs
- Before changing reading, tokenizing, or process launching, read `docs/command-loop.md`.
- Before adding or changing a builtin or history, read `docs/builtins.md`.

## Planned
- Before adding tests, linting, or CI, read `docs/specs/test-suite.md`.
