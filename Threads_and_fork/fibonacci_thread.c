#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Function to generate Fibonacci series
void* generateFibonacci(void* arg) {
    int n = *(int*)arg; // Get the number of Fibonacci numbers to generate
    int t1 = 0, t2 = 1, nextTerm;

    printf("Fibonacci Series: ");
    for (int i = 1; i <= n; i++) {
        printf("%d ", t1); // Print the current term
        nextTerm = t1 + t2; // Calculate next term
        t1 = t2; // Update terms
        t2 = nextTerm; // Update terms
    }
    printf("\n"); // New line after series
    return NULL; // End thread
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <number_of_terms>\n", argv[0]); // Check for user input
        return 1; // Exit with error
    }

    int n = atoi(argv[1]); // Convert input to integer
    pthread_t thread;

    // Create a thread to generate Fibonacci series
    pthread_create(&thread, NULL, generateFibonacci, &n);
    
    // Wait for the thread to finish
    pthread_join(thread, NULL);

    return 0; // Main thread ends
}

/*
Fibonacci Series: 0 1 1 2 3 5 8 13 21 34

*/