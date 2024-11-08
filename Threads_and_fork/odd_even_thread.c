// gcc p.c -o code -pthread


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Function to print odd numbers from 1 to 1000
void* printOdd(void* arg) {
    for (int i = 1; i <= 1000; i += 2) {
        printf("Odd: %d\n", i); // Print odd number
    }
    return NULL; // End thread
}

// Function to print even numbers from 1 to 1000
void* printEven(void* arg) {
    for (int i = 2; i <= 1000; i += 2) {
        printf("Even: %d\n", i); // Print even number
    }
    return NULL; // End thread
}

int main() {
    pthread_t threadA, threadB;

    // Create thread A for printing odd numbers
    pthread_create(&threadA, NULL, printOdd, NULL);
    
    // Create thread B for printing even numbers
    pthread_create(&threadB, NULL, printEven, NULL);
    
    // Wait for both threads to finish
    pthread_join(threadA, NULL); // Wait for thread A
    pthread_join(threadB, NULL); // Wait for thread B

    return 0; // Main thread ends
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