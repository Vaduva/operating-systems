/**
 * 2 process. Parent displays 'A' and child displays 'B'. After 7 seconds
 * alarm triggers on parent which triggers SIGUSR1 on child. Child
 * terminates, parent gets notified and terminates also.
 */

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int pid;
struct sigaction s;

void alarm_handler() {
    // Send SIGUSR1 to child process
    if (kill(pid, SIGUSR1) == -1) {
        printf("Error sending SIGUSR1 to child\n");
        exit(1);
    }
}

void terminate_child_handler() {
    // Terminate child process
    printf("Child process terminated\n");
    exit(0);
}

void terminate_parent_handler() {
    // Terminate parent process
    printf("Parent process terminated\n");
    exit(0);
}

int main() {
    // Create child process
    pid = fork();

    if (pid == -1) {
        printf("Fork creation error\n");
        exit(1);
    }

    if (pid == 0) {
        // In child process

        sigset_t signalset;

        // Initialize signalset as empty
        sigemptyset(&signalset);

        // Add SIGINT to signalset
        sigaddset(&signalset, SIGINT);

        // Block signals from signalset (SIGINT, that is)
        if (sigprocmask(SIG_BLOCK, &signalset, NULL) == -1){
            printf("Error blocking SIGINT signal\n");
            exit(1);
        }

        s.sa_handler = SIG_IGN;
        if (sigaction(SIGINT, &s, 0) == -1) {
            printf("Error setting handler for child SIGINT\n");
            exit(1);
        }

        // Set a handler for parent triggered SIGUSR1 signal
        s.sa_handler = terminate_child_handler;
        if (sigaction(SIGUSR1, &s, 0) == -1) {
            printf("Error setting handler for child SIGUSR1\n");
            exit(1);
        }

        // Display in loop
        while(1) {
            usleep(1000000);
            printf("B\n");
        }
    } else {
        // In parent process

        // Send a SIGALRM signal after 7 seconds
        alarm(7);

        // Set a handler for the SIGALRM signal
        s.sa_handler = alarm_handler;
        if (sigaction(SIGALRM, &s, 0) == -1) {
            printf("Error setting handler for parent SIGALRM\n");
            exit(1);
        }

        // Set a handler for when child process terminates (SIGCHLD triggered)
        s.sa_handler = terminate_parent_handler;
        if (sigaction(SIGCHLD, &s, 0) == -1) {
            printf("Error setting handler for parent SIGCHLD\n");
            exit(1);
        }

        // Display in loop
        while(1) {
            usleep(1000000);
            printf("A\n");
        }
    }
    return 0;
}
