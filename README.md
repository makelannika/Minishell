# MiniShell

## Overview

**MiniShell** is a lightweight shell that mimics various features of Bash, including handling piped commands, redirects, heredoc, expansion of environment variables, signal handling, and several built-in commands. The built-in commands supported are:

- `echo`
- `pwd`
- `unset`
- `export`
- `cd`
- `env`
- `exit`

When executed, MiniShell creates an interactive shell environment where you can run most standard shell commands, including the ability to run subshells within the shell itself.

## Installation

To install MiniShell, use Docker to build the environment:

```docker build .```

The build process will download the necessary dependencies and set up the Docker container. This may take a few minutes, depending on your internet speed.

## Running MiniShell
After the installation is complete, you can run the program using:

```./minishell```
## Exiting the Shell
To exit MiniShell, you can either:

Type the ```exit``` command or press Ctrl+D.
Please note that the usual Ctrl+C command for terminating a process won’t work as expected within MiniShell. This is because signal handling is managed similarly to how Bash handles it.

## Additional Notes
Subshells: MiniShell supports running subshells, allowing you to execute commands in nested shell environments.
Signal Handling: Special care has been taken to intercept and manage signals like Ctrl+C, ensuring that they behave similarly to a standard Bash shell.
