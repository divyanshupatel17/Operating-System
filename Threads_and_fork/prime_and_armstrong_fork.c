#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>

// Function to check if a number is prime
int isPrime(int num) {
    if (num < 2) return 0; // Numbers less than 2 are not prime
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) return 0; // Not prime if divisible
    }
    return 1; // Return true for prime
}

// Function to print prime numbers from 1 to 1000
void printPrimes() {
    for (int i = 1; i <= 1000; i++) {
        if (isPrime(i)) {
            printf("Prime: %d\n", i); // Print prime number
        }
    }
}

// Function to check if a number is Armstrong
int isArmstrong(int num) {
    int sum = 0, temp = num, digits = 0;

    // Count the number of digits
    while (temp) {
        digits++;
        temp /= 10; // Reduce the number
    }

    temp = num; // Reset temp to original number
    // Calculate the sum of digits raised to the power of digits
    while (temp) {
        sum += pow(temp % 10, digits); // Calculate Armstrong sum
        temp /= 10; // Reduce the number
    }
    return sum == num; // Check if it matches original number
}

// Function to print Armstrong numbers from 1 to 1000
void printArmstrongs() {
    for (int i = 1; i <= 1000; i++) {
        if (isArmstrong(i)) {
            printf("Armstrong: %d\n", i); // Print Armstrong number
        }
    }
}

int main() {
    pid_t pidA, pidB;

    // Create first child process A for prime numbers
    pidA = fork();
    if (pidA == 0) {
        printPrimes(); // Child process A executes printPrimes
        exit(0); // End child process A
    }

    // Create second child process B for Armstrong numbers
    pidB = fork();
    if (pidB == 0) {
        printArmstrongs(); // Child process B executes printArmstrongs
        exit(0); // End child process B
    }

    // Parent process waits for both child processes to finish
    wait(NULL); // Wait for child A
    wait(NULL); // Wait for child B

    return 0; // Main process ends
}

/*
Prime: 2
Prime: 3
...
Armstrong: 1
Armstrong: 153
...

*/