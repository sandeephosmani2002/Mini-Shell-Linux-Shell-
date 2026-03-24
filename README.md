# MiniShell

MiniShell is a Linux-based command line interpreter implemented in C. It mimics basic functionalities of a UNIX shell by allowing users to execute internal and external commands.

## Features
- Execution of external Linux commands
- Built-in commands support
- Command parsing
- Pipe handling (`|`)
- Input and output redirection
- Background process execution
- Job control commands (`jobs`, `fg`, `bg`)
- Signal handling (`Ctrl+C`, `Ctrl+Z`)
- Process creation using `fork()`
- Command execution using `exec()` family

## Technologies Used
- C Programming
- Linux
- POSIX System Calls

## Project Files
- `main.c` – Main shell program
- `scan_input.c` – Reads and processes user input
- `command_type.c` – Identifies command type
- `get_command.c` – Extracts commands from input
- `execute_internal_commands.c` – Handles built-in commands
- `execute_external_cmds.c` – Executes external commands
- `extract_external_commands.c` – Loads external command list
- `header.h` – Header file
- `external_commands.txt` – List of supported external commands
- `Makefile` – Compilation instructions

## Compilation
```bash
make
```
## Run
```bash
./minishell
```
## Author

SANDEEP HOSMANI
