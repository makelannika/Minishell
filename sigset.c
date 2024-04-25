#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
	struct stat st;
    printf("Size of sigset_t: %zu bytes\n", sizeof(sigset_t));

	  // Get information about a file
    if (stat("commans.txt", &st) == 0) {
        printf("File size: %lld bytes\n", st.st_size);
    } else {
        perror("stat() error");
        return -1;
    }

    // Delete a file
    if (unlink("/tmp/example.txt") == 0) {
        printf("File deleted successfully\n");
    } else {
        perror("unlink() error");
        return -1;
    }
}