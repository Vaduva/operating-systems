/**
 * Parent process reads chars from given file (first argument) to pipe;
 * child process reads chars from pipe, and exits with number of digits found
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Not enough arguments\n");
        exit(1);
    }

    int pip[2];
    int pid;

    // Create pipe
    if (pipe(pip) == -1) {
        printf("Pipe creation error\n");
        exit(1);
    }

    // Create child process
    pid = fork();
    if (pid == -1) {
        printf("Fork creation error\n");
        exit(1);
    }

    // In child process
    if (pid == 0) {
        // Close write end of pipe
        close(pip[1]);

        char byte;
        int digit_count = 0;

        // Read from pipe
        while (read(pip[0], &byte, 1)) {
            if (isdigit(byte)) {
                digit_count++;
            }
        }

        // Close the other end too
        close(pip[0]);

        // Exit with number of digits
        exit(digit_count);
    }

    // Close read end of pipe
    close(pip[0]);

    // In parent process
    int file = open(argv[1], O_RDONLY);

    if (file == -1) {
        printf("Given file \"%s\" could not be opened\n", argv[1]);
        exit(1);
    }

    // Get filesystem blocksize
    int block_size;
    struct stat s;
    if (stat(argv[1], &s) < 0) {
        printf("Information for file \"%s\" could not be retrieved\n", argv[1]);
        exit(1);
    }
    block_size = s.st_blksize;

    // Set number of chars to be read from file
    char *bytes = malloc(block_size * sizeof(char));

    // Read given file char by char
    while (read(file, bytes, block_size)) {
        // Write each char to pipe
        if (write(pip[1], bytes, block_size) == -1) {
            printf("Could not write char from file to pipe\n");
            exit(1);
        }
    }

    // Close file
    close(file);

    // Close the other end too
    close(pip[1]);

    // Wait for child process to terminate
    int status;
    wait(&status);

    if (WIFEXITED(status)) {
        printf("Child process ended with: %d\n", WEXITSTATUS(status));
    }

    return 0;
}
