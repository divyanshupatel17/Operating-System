#include <stdio.h>
#include <stdbool.h>

#define n 5 // Number of processes
#define m 3 // Number of resources

// Function to calculate the Need matrix
void calculateNeed(int need[n][m], int max[n][m], int alloc[n][m]) {
    // Calculate the Need matrix by subtracting the Allocation matrix from the Max matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - alloc[i][j]; // Need = Max - Alloc
        }
    }
}

// Function to check if the system is in a safe state
bool isSafe(int avail[m], int max[n][m], int alloc[n][m]) {
    // Step 1: Calculate the Need matrix
    int need[n][m];
    calculateNeed(need, max, alloc);

    // Step 2: Initialize the Finish array and Safe sequence
    bool finish[n] = {false}; // Finish array to track completed processes
    int safeSeq[n];           // Array to store the safe sequence of processes
    int count = 0;            // Count of processes that have finished

    // Step 3: Check for a safe sequence
    while (count < n) { // Continue until all processes are finished
        bool found = false; // Flag to check if a process was found
        for (int p = 0; p < n; p++) { // Iterate through all processes
            if (!finish[p]) { // If process p is not finished
                // Step 4: Check if resources can be allocated to process p
                bool canProceed = true; // Flag to check if process can proceed
                for (int j = 0; j < m; j++) { // Check each resource
                    if (need[p][j] > avail[j]) { // If Need > Available
                        canProceed = false; // Cannot proceed
                        break; // Exit loop
                    }
                }
                // Step 5: If process can proceed
                if (canProceed) {
                    // Simulate resource allocation
                    for (int j = 0; j < m; j++) {
                        avail[j] += alloc[p][j]; // Release allocated resources
                    }

                    finish[p] = true; // Mark process as finished
                    safeSeq[count++] = p; // Add process to safe sequence
                    found = true; // A process has been found
                }
            }
        }
        // Step 6: If no process can proceed, the system is not in a safe state
        if (!found) {
            printf("The system is not in a safe state.\n");
            return false; // Not in a safe state
        }
    }

    // Step 7: Print the safe sequence
    printf("Following is the SAFE Sequence:\n");
    for (int i = 0; i < n; i++) {
        printf(" P%d", safeSeq[i]); // Print each process in the safe sequence
        if (i < n - 1) printf(" ->"); // Print arrow for continuity
    }
    printf("\n");
    return true; // System is in a safe state
}

// Function to request resources for a process
bool requestResources(int pid, int request[m], int avail[m], int max[n][m], int alloc[n][m]) {
    // Step 1: Calculate the Need matrix
    int need[n][m];
    calculateNeed(need, max, alloc);

    // Step 2: Check if request is less than or equal to the Need
    for (int i = 0; i < m; i++) {
        if (request[i] > need[pid][i]) {
            printf("Error: Process has exceeded its maximum claim.\n");
            return false; // Request exceeds maximum claim
        }
    }

    // Step 3: Check if request is less than or equal to the Available
    for (int i = 0; i < m; i++) {
        if (request[i] > avail[i]) {
            printf("Resources are not available right now.\n");
            return false; // Resources not available
        }
    }

    // Step 4: Pretend to allocate the resources
    for (int i = 0; i < m; i++) {
        avail[i] -= request[i]; // Decrease available resources
        alloc[pid][i] += request[i]; // Allocate resources to the process
        need[pid][i] -= request[i]; // Decrease the need of the process
    }

    // Step 5: Check if the system is still in a safe state
    if (isSafe(avail, max, alloc)) {
        return true; // Resources allocated successfully
    } else {
        // Step 6: Rollback if the system would be unsafe
        for (int i = 0; i < m; i++) {
            avail[i] += request[i]; // Restore available resources
            alloc[pid][i] -= request[i]; // Deallocate resources from the process
            need[pid][i] += request[i]; // Restore the need of the process
        }
        printf("Request cannot be granted as it leaves the system in an unsafe state.\n");
        return false; // System would be unsafe
    }
}

int main() {
    // Step 1: Define the Allocation Matrix
    int alloc[n][m] = {
        {0, 1, 0}, // P0
        {2, 0, 0}, // P1
        {3, 0, 2}, // P2
        {2, 1, 1}, // P3
        {0, 0, 2}  // P4
    };

    // Step 2: Define the MAX Matrix
    int max[n][m] = {
        {7, 5, 3}, // P0
        {3, 2, 2}, // P1
        {9, 0, 2}, // P2
        {2, 2, 2}, // P3
        {4, 3, 3}  // P4
    };

    // Step 3: Define the Available Resources
    int avail[m] = {3, 3, 2}; // Available Resources

    // Step 4: Check if the system is in a safe state initially
    isSafe(avail, max, alloc);

    // Step 5: Example resource request by process P1
    int request[m] = {1, 0, 2}; // Request by process P1 for 1 unit of R1 and 2 units of R2
    int pid = 1; // Process ID requesting resources

    // Step 6: Attempt to allocate resources
    if (requestResources(pid, request, avail, max, alloc)) {
        printf("Resources allocated successfully.\n");
    }

    return 0; // End of program
}
/**
Following is the SAFE Sequence:
 P1 -> P3 -> P4 -> P0 -> P2
Following is the SAFE Sequence:
 P0 -> P1 -> P2 -> P3 -> P4
Resources allocated successfully.

 */
