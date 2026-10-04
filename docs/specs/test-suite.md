# Test suite, lint, and CI

**Status:** draft

`make test` and `make lint` exist, run locally and in CI, and catch regressions in parsing, builtins, and process launching.

## Goal

There is no automated check today; the only verification is running the shell by hand.
Known bugs (EOF loops forever, `make clean` is broken) went unnoticed because nothing exercises them.
Success: every change runs unit tests, end-to-end tests, sanitizers, and strict compiler warnings, locally and on every push.

## Scope

- In: unit tests for pure functions, end-to-end tests driving the binary over stdin, strict warnings, formatting check, sanitizer build, GitHub Actions on Linux and macOS, regression tests and fixes for the two known bugs.
- Out: new shell features (pipes, quoting, signals), coverage thresholds.

## Design

**Testable units.** Move `main` from `simple_shell.c` into `main.c` so tests can link the rest without a duplicate `main`.
Expose `simple_shell_split_line` and `simple_shell_execute` through a new `simple_shell.h`.

**Unit tests** (`tests/unit/*.c`): a minimal assert harness in `tests/unit/test.h` (no external dependency), one executable per file, linked against the shell objects.
Targets: `split_line` (empty, whitespace-only, tabs, more than 64 tokens to force a realloc), `add_to_history` (wrap at 100, oldest freed), builtin table alignment.

**End-to-end tests** (`tests/e2e/*.bats`): [bats-core](https://github.com/bats-core/bats-core) pipes input into `./simple_shell` and asserts on stdout, stderr, and exit status.
Each test is wrapped in `timeout` so an infinite loop fails instead of hanging.
Cases: external command, unknown command error, `cd` then `pwd`, `cd` with no args, `help` lists every builtin, `history` numbering, `exit`, EOF exits cleanly.

**Lint.**

- `CFLAGS` gains `-Wextra -Werror -std=c11 -pedantic` (fix the unused-parameter warnings in `exit.c`, `history.c`, and `main`).
- `.clang-format` checked with `clang-format --dry-run --Werror`.
- `make sanitize` builds with `-fsanitize=address,undefined` and the test targets run against it.

**Makefile.** Replace the hand-listed rules with a pattern rule and an `OBJS` variable, so new files need one line; add `test`, `test-unit`, `test-e2e`, `lint`, `sanitize`, and a working `clean`.

**CI.** `.github/workflows/ci.yml` runs `make lint` and `make test` on `ubuntu-latest` and `macos-latest`.

## Tasks

- [ ] Add `bats` E2E test for EOF; confirm it fails (times out) on current code.
- [ ] Fix EOF handling in `simple_shell_read_line` / loop so EOF exits with status 0; test passes.
- [ ] Fix `make clean` and refactor `Makefile` to `OBJS` plus a pattern rule.
- [ ] Split `main` into `main.c`; add `simple_shell.h`.
- [ ] Add unit harness and unit tests; `make test-unit` passes.
- [ ] Add remaining E2E cases; `make test-e2e` passes.
- [ ] Tighten `CFLAGS`, add `.clang-format`, fix warnings and formatting; `make lint` passes.
- [ ] Add `make sanitize`; tests pass under ASan and UBSan.
- [ ] Add GitHub Actions workflow; green on both OSes.

## Done when

- [ ] `make lint` and `make test` pass locally and in CI on Linux and macOS.
- [ ] EOF regression test and `make clean` both pass.
- [ ] `docs/testing.md` written from the docs template; `docs/command-loop.md` gotchas updated for the EOF fix.
- [ ] `AGENTS.md` Commands list `make test`, `make lint`, and a single-test command for both unit and bats; Repo map and Docs updated.
- [ ] This spec deleted and removed from the Planned index.

## Open questions

- LeakSanitizer is unsupported on Apple Silicon; run leak checks only in the Linux CI job? Default: yes, macOS runs ASan without `detect_leaks`.
- Call `free_history` on exit so leak checks are clean, or suppress it? Default: call it.

## Related

- [Command loop](../command-loop.md)
- [Builtins](../builtins.md)
