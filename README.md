# Mini-Shell-Linux-Shell-
A C-based Mini Shell that simulates basic UNIX shell functionality. This project supports executing Linux commands, built-in commands, and job control using system calls like fork(), execvp(), and wait().
📌 Features

Executes external Linux commands

Supports built-in commands:

cd

pwd

echo

exit

clear

Supports job control commands:

jobs

fg

bg

Supports running commands in foreground and background

Handles signals:

Ctrl + C (SIGINT)

Ctrl + Z (SIGTSTP)

Displays a custom shell prompt

🗂️ Project Structure

Mini_Shell

├── main.c # Program entry point
├── shell.c # Main shell loop and command execution
├── builtins.c # Built-in commands implementation
├── jobs.c # Background job management (fg, bg, jobs)
├── signals.c # Signal handling (Ctrl+C, Ctrl+Z)
└── header.h # Function prototypes and structures

⚙️ Compilation

Use gcc to compile all source files:

gcc *.c

🧪 Usage

Run the compiled program:

./a.out

Example Commands
ls
pwd
cd Documents
sleep 10 &
jobs
fg 1

🧠 Concepts Used

Process Management (fork(), execvp(), wait())

Signals (SIGINT, SIGTSTP)

File Descriptors

Pipes and Redirection (if implemented)

Dynamic Memory Allocation

Data Structures for Job Control

📚 Learning Outcome

This project helps in understanding how a real Linux shell works internally, including:

Process creation and execution

Foreground/background process handling

Signal handling and job control

Implementing built-in shell commands

👨‍💻 Author
***SANDEEP HOSMANI***
