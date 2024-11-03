// Banker's Algorithm

#include <stdio.h>

int main() {
    int n = 5; // Number of processes
    int m = 3; // Number of resources

    // Allocation Matrix
    int alloc[5][3] = {
        {0, 1, 0}, // P0
        {2, 0, 0}, // P1
        {3, 0, 2}, // P2
        {2, 1, 1}, // P3
        {0, 0, 2}  // P4
    };

    // MAX Matrix
    int max[5][3] = {
        {7, 5, 3}, // P0
        {3, 2, 2}, // P1
        {9, 0, 2}, // P2
        {2, 2, 2}, // P3
        {4, 3, 3}  // P4
    };

    int avail[3] = {3, 3, 2}; // Available Resources
    int f[n], ans[n], ind = 0;  // answer array for safe sequence and index

    // f[] is the "finish" array
    // It tracks whether a process has been executed and finished
    // f[i] = 0 means process Pi hasn't finished yet
    // f[i] = 1 means process Pi has completed
    for (int k = 0; k < n; k++) f[k] = 0;

    // Calculate Need Matrix
    int need[n][m];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];

    // Banker's Algorithm Logic
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            if (f[i] == 0) { // Process Pi hasn't finished yet
                int flag = 0;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > avail[j]) {
                        flag = 1; // Not enough resources available for Pi
                        break;
                    }
                }
                if (flag == 0) {
                    // Process Pi can be completed safely
                    ans[ind++] = i; // Store process Pi in the safe sequence
                    for (int y = 0; y < m; y++)
                        avail[y] += alloc[i][y]; // Release resources after Pi finishes
                    f[i] = 1; // Mark Pi as finished
                }
            }
        }
    }

    // Check if the system is safe
    int isSafe = 1;
    for (int i = 0; i < n; i++) {
        if (f[i] == 0) {
            isSafe = 0; // System is not safe
            printf("The following system is not safe\n");
            break;
        }
    }

    // Print the safe sequence if it exists
    if (isSafe) {
        printf("Following is the SAFE Sequence:\n");
        for (int i = 0; i < n - 1; i++)
            printf(" P%d ->", ans[i]);
        printf(" P%d\n", ans[n - 1]);
    }
    return 0;
}

/*
Explanation of variables:
- `f[]`: This array tracks whether each process has finished (1) or not (0).
- `ans[]`: This array stores the sequence of processes that form a safe sequence (if possible).
- `ind`: This is an index variable to insert the next process into the `ans[]` array when a process safely completes.
*/

