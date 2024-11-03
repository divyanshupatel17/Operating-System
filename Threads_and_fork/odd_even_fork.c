#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Function to print odd numbers from 1 to 1000
void printOdd() {
    for (int i = 1; i <= 1000; i += 2) {
        printf("Odd: %d\n", i); // Print odd number
    }
}

// Function to print even numbers from 1 to 1000
void printEven() {
    for (int i = 2; i <= 1000; i += 2) {
        printf("Even: %d\n", i); // Print even number
    }
}

int main() {
    pid_t pidA, pidB;

    // Create first child process A for odd numbers
    pidA = fork();
    if (pidA == 0) {
        printOdd(); // Child process A executes printOdd
        exit(0); // End child process A
    }

    // Create second child process B for even numbers
    pidB = fork();
    if (pidB == 0) {
        printEven(); // Child process B executes printEven
        exit(0); // End child process B
    }

    // Parent process waits for both child processes to finish
    wait(NULL); // Wait for child A
    wait(NULL); // Wait for child B

    return 0; // Main process ends
}

/*
Odd: 1
Even: 2
Odd: 3
Even: 4
...
Odd: 999
Even: 1000

*/