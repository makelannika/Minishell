// #include <stdio.h>
// #include <stdlib.h>
// #include <term.h>
// #include <termios.h>
// #include <unistd.h>
// #include <curses.h>
#include "minishell.h"

int main() {
    // Initialize the terminal database
    if (tgetent(NULL, getenv("TERM")) != 1) {
        fprintf(stderr, "Unable to initialize terminal database.\n");
        return 1;
    }

    // Retrieve and print boolean flag
    int bold = tgetflag("bold");
    printf("Bold attribute: %s\n", bold ? "Enabled" : "Disabled");

    // Retrieve and print numeric value
    int cols = tgetnum("cols");
    int lns = tgetnum("lines");
    printf("Terminal size: %d columns x %d lines\n", cols, lns);

    // Retrieve and print string value
    char *cursor_movement = tgetstr("cm", NULL);
    if (cursor_movement != NULL) {
        printf("Cursor movement sequence: %s\n", cursor_movement);
    } else {
        fprintf(stderr, "Unable to retrieve cursor movement sequence.\n");
    }

    // Set terminal attributes
    struct termios term_attr;
    if (tcgetattr(STDIN_FILENO, &term_attr) == -1) {
        perror("tcgetattr");
        return 1;
    }
    term_attr.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_attr) == -1) {
        perror("tcsetattr");
        return 1;
    }

    // Use the terminal with modified attributes
    printf("Terminal attributes modified. Press 'q' to quit.\n");
    while (getchar() != 'q');

    return 0;
}