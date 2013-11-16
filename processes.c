/**
 * Start m processes that count to n and another one that executes ls -l on
 * dir. m, n and dir are given arguments
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Not enough arguments!\n");
        exit(1);
    }

    // Parse given arguments
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);

    // Start m child processes
    int i, pid;
    for (i = 0; i < m; i++) {
        pid = fork();

        if (pid == -1) {
            printf("Error");
            exit(1);
        }
        if (pid == 0) {
            // Print count for each child process
            int j;
            for (j = 1; j <= n; j++) {
                printf("%d:%d\n", getpid(), j);
            }
            exit(n);
        }
    }

    // Start new process and execute command
    pid = vfork();
    if (pid == -1) {
        printf("Error");
        exit(1);
    }
    if (pid == 0) {
        char *newargv[] = {"ls", "-l", argv[3], NULL};
        char *newenviron[] = {NULL};
        execve("/bin/ls", newargv, newenviron);
        perror("execve");
        exit(EXIT_FAILURE);
    }

    // Wait for each child process to terminate and output it's status
    for (i = 0; i < m+1; i++) {
        int status;
        pid = wait(&status);
        if (WIFEXITED(status)) {
            printf("Ended %d:%d\n", pid, WEXITSTATUS(status));
        }
    }

    return 0;
}
