// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <readline/readline.h>
// #include <readline/history.h>
// #include <unistd.h>
#include "minishell.h"

int main() {
    char *input;

    // Readline loop
    while (1) {
        // Read a line of input

        input = readline("Enter a command: ");

        // Check for EOF or user typing 'exit'
        if (!input || strcmp(input, "exit") == 0) {
            printf("Exiting...\n");
            free(input);
            break;
        }

        // Add input to history
        add_history(input);

        // Notify readline that a new line is starting
        rl_on_new_line();

        // Replace the current line of input with a new line
        rl_replace_line("Processing input...", 0);

        // Redisplay the updated line
        rl_redisplay();

        // Process input (in this example, just echo back)
        printf("You entered: %s\n", input);

        // Free input buffer
        free(input);
    }

    // Clear history
    rl_clear_history();

    return 0;
}
