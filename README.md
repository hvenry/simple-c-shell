# Simple C Shell

![screenshot](screenshots/simple-c-shell.png?raw=true "Simple C Shell")

## Built-in Commands:
- `cd`: Change the current directory.
- `help`: Display information about the shell and built-in commands.
- `exit`: Exit the shell.
- `history`: View the list of previously executed commands during the current session.

## External Command Execution:

Supports executing any external command available in the system's PATH, such as `ls`, `pwd`, `echo`, etc.
  
## Getting Started

### Prerequisites

- **C Compiler**: You need `GCC`, `Clang`, or any other C compiler to build the shell.
- **Unix-like Environment**: This shell is designed to work on Unix-like systems (Linux, macOS).

### Setup

1. **Clone the Repository**:
   ```bash
   git clone git@github.com:hvenry/simple-c-shell.git
   ```
2. **CD into root of repo**:
   ```bash
   cd simple-c-shell
   ```
3. **Run the Makefile**:
   ```bash
   make
   ```
4. **Start the shell**:
   ```bash
   ./simple_shell
   ```
By now you should be using the shell, type `help` for a list of commands and `exit` to terminate the shell. Enjoy!

[Credit](https://brennan.io/2015/01/16/write-a-shell-in-c/)
