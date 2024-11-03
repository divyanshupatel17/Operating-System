#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

// Function to check if a number is prime
int isPrime(int num) {
    if (num < 2) return 0; // Numbers less than 2 are not prime
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) return 0; // Not prime if divisible
    }
    return 1; // Return true for prime
}

// Function to print prime numbers from 1 to 1000
void* printPrimes(void* arg) {
    for (int i = 1; i <= 1000; i++) {
        if (isPrime(i)) {
            printf("Prime: %d\n", i); // Print prime number
        }
    }
    return NULL; // End thread
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
void* printArmstrongs(void* arg) {
    for (int i = 1; i <= 1000; i++) {
        if (isArmstrong(i)) {
            printf("Armstrong: %d\n", i); // Print Armstrong number
        }
    }
    return NULL; // End thread
}

int main() {
    pthread_t threadA, threadB;

    // Create thread A for printing prime numbers
    pthread_create(&threadA, NULL, printPrimes, NULL);
    
    // Create thread B for printing Armstrong numbers
    pthread_create(&threadB, NULL, printArmstrongs, NULL);
    
    // Wait for both threads to finish
    pthread_join(threadA, NULL); // Wait for thread A
    pthread_join(threadB, NULL); // Wait for thread B

    return 0; // Main thread ends
}

/*
Prime: 2
Prime: 3
...
Armstrong: 1
Armstrong: 153
...

*/