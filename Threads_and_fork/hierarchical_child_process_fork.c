#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pidA, pidB, pidC;

    // Create first child process A
    pidA = fork();
    if (pidA == 0) {
        // Process A
        printf("Process A: PID = %d, Parent PID = %d\n", getpid(), getppid());

        // Create second child process B
        pidB = fork();
        if (pidB == 0) {
            // Process B
            printf("Process B: PID = %d, Parent PID = %d\n", getpid(), getppid());

            // Create third child process C
            pidC = fork();
            if (pidC == 0) {
                // Process C
                printf("Process C: PID = %d, Parent PID = %d\n", getpid(), getppid());
                exit(0); // End child process C
            }
            wait(NULL); // Wait for child C to finish
            exit(0); // End child process B
        }
        wait(NULL); // Wait for child B to finish
        exit(0); // End child process A
    }

    wait(NULL); // Wait for child A to finish
    return 0; // Main process ends
}


/*
Process A: PID = 1234, Parent PID = 1233
Process B: PID = 1235, Parent PID = 1234
Process C: PID = 1236, Parent PID = 1235

*/