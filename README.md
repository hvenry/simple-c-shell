# Simple C Shell

A minimal Unix shell in C that reads, parses, and executes commands, with `cd`, `help`, `exit`, and `history` builtins.

![Simple C Shell running help](screenshots/simple-c-shell.png?raw=true "Simple C Shell")

## Why
Built to understand what a shell actually does under the hood: the read-parse-execute loop, `fork`/`execvp`/`waitpid`, and why some commands must run inside the shell process.
It is deliberately small, so every stage fits in one readable function.
Based on Stephen Brennan's [Write a Shell in C](https://brennan.io/2015/01/16/write-a-shell-in-c/).

## Quick start
Needs a C compiler and a Unix-like OS (Linux or macOS).
```bash
git clone git@github.com:hvenry/simple-c-shell.git
cd simple-c-shell
make
./simple_shell
```
Type `help` for the builtins and `exit` to quit.

## Docs
- [Command loop](docs/command-loop.md) - how a line becomes a builtin call or a child process, and current limits
- [Builtins](docs/builtins.md) - what each builtin does and how to add one
- [AGENTS.md](AGENTS.md) - commands, conventions, and the full docs index

## Status
Learning project, not a daily-driver shell: no pipes, redirection, quoting, or signal handling.
Ctrl-D does not exit yet (use `exit`); a test suite and that fix are planned in [docs/specs/test-suite.md](docs/specs/test-suite.md).
