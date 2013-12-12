/**
 * Parent process executes `ls -l dir`, where `dir` is a directory given by
 * argument. The result of this execution is passed to the first child which
 * prints all directories and then sends this list to the second child, which,
 * in turn, counts the number of lines he received and prints it on the screen
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Not enough arguments supplied\n");
        exit(1);
    }

    int pip1[2];
    if (pipe(pip1) == -1) {
        printf("Could not create first pipe\n");
        exit(1);
    }

    int pip2[2];
    if (pipe(pip2) == -1) {
        printf("Could not create second pipe\n");
        exit(1);
    }

    int pid1 = fork();
    if (pid1 == -1) {
        printf("Could not create first child process\n");
        exit(1);
    }

    int pid2 = fork();
    if (pid2 == -1) {
        printf("Could not create second child process\n");
        exit(1);
    }

    if (pid1 == 0) {
        // In first child process

        // First child reads from first pipe and writes to second pipe
        close(pip1[1]);
        close(pip2[0]);

        // Redirect first pipe input to stdin
        dup2(pip1[0], 0);

        // Read lines from stdin (first pipe, that is)
        char line[100];
        while (fgets(line, 100, stdin)) {
            // Check if line is directory
            if (line[0] == 'd') {
                // If it is, print line on screen and write to second pipe
                printf("%s", line);
                write(pip2[1], line, strlen(line));
            }
        }

        // Close other ends of pipes also
        close(pip1[0]);
        close(pip2[1]);

        exit(0);
    } else if (pid2 == 0) {
        // In second child process

        // Second child reads from second pipe noly
        close(pip1[0]);
        close(pip1[1]);
        close(pip2[1]);

        // Redirect second pipe input to stdin
        dup2(pip2[0], 0);

        // Read lines from stdin (second pipe, that is)
        char line[100];
        int i;
        for (i = 0; fgets(line, 100, stdin); i++);

        // Print number of read lines on screen
        printf("Number of lines: %d\n", i);

        // Close the other end of the second pipe
        close(pip2[0]);

        exit(0);
    } else {
        // In parent process

        // Parent writes to first pipe only
        close(pip1[0]);
        close(pip2[0]);
        close(pip2[1]);

        // Redirect stdout to first pipe output
        dup2(pip1[1], 1);

        // Execute `ls -l argv[1]` (which prints to stdout)
        execlp("ls", "ls", "-l", argv[1], NULL);
    }

    return 0;
}
