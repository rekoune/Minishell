# Minishell

Minishell is a minimalistic shell implementation in C. It aims to replicate core functionalities of common Unix shells, providing a learning experience for understanding system calls, process management, and shell scripting behaviors.

## Features

- Command parsing and execution
- Environment variable management
- Built-in commands (`cd`, `exit`, `echo`, etc.)
- Input/output redirection
- Piping between commands
- Signal handling (e.g., `Ctrl+C`, `Ctrl+D`)
- Error handling and reporting

## Getting Started

### Prerequisites

- GCC or Clang (C99 or later)
- Unix-like operating system (Linux, macOS)
- Make

### Build Instructions

1. Clone the repository:

    ```bash
    git clone https://github.com/rekoune/Minishell.git
    cd Minishell
    ```

2. Compile the project:

    ```bash
    make all clean
    ```

3. Run the shell:

    ```bash
    ./minishell
    ```

## Usage

- Type commands as you would in a standard shell.
- Use built-in commands or external programs.
- Use pipes (`|`) and redirections (`>`, `<`).

Example:

```bash
echo "Hello, World!" | grep World > output.txt
```
