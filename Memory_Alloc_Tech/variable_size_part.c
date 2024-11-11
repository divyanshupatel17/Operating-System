// this is variable size : 

#include <stdio.h>
#include <limits.h>

#define MAX_PARTITIONS 5
#define MAX_PROCESSES 4

void firstFit(int partitions[], int n_partitions, int processes[], int n_processes) {
    int allocation[MAX_PROCESSES];
    
    // Initialize allocation array with -1
    for (int i = 0; i < n_processes; i++)
        allocation[i] = -1;
    
    // Allocate memory to each process using First Fit
    for (int i = 0; i < n_processes; i++) {
        for (int j = 0; j < n_partitions; j++) {
            if (partitions[j] >= processes[i]) {
                allocation[i] = j;
                partitions[j] -= processes[i];
                break;
            }
        }
    }
    
    // Print allocation results
    printf("\nFirst Fit Allocation:\n");
    for (int i = 0; i < n_processes; i++) {
        printf("Process %d: ", i + 1);
        if (allocation[i] != -1)
            printf("Allocated to Partition %d\n", allocation[i] + 1);
        else
            printf("Not Allocated\n");
    }
}

void bestFit(int partitions[], int n_partitions, int processes[], int n_processes) {
    int allocation[MAX_PROCESSES];
    
    // Initialize allocation array with -1
    for (int i = 0; i < n_processes; i++)
        allocation[i] = -1;
    
    // Allocate memory to each process using Best Fit
    for (int i = 0; i < n_processes; i++) {
        int best_idx = -1;
        int min_diff = INT_MAX;
        
        for (int j = 0; j < n_partitions; j++) {
            if (partitions[j] >= processes[i]) {
                if (partitions[j] - processes[i] < min_diff) {
                    min_diff = partitions[j] - processes[i];
                    best_idx = j;
                }
            }
        }
        
        if (best_idx != -1) {
            allocation[i] = best_idx;
            partitions[best_idx] -= processes[i];
        }
    }
    
    // Print allocation results
    printf("\nBest Fit Allocation:\n");
    for (int i = 0; i < n_processes; i++) {
        printf("Process %d: ", i + 1);
        if (allocation[i] != -1)
            printf("Allocated to Partition %d\n", allocation[i] + 1);
        else
            printf("Not Allocated\n");
    }
}

void worstFit(int partitions[], int n_partitions, int processes[], int n_processes) {
    int allocation[MAX_PROCESSES];
    
    // Initialize allocation array with -1
    for (int i = 0; i < n_processes; i++)
        allocation[i] = -1;
    
    // Allocate memory to each process using Worst Fit
    for (int i = 0; i < n_processes; i++) {
        int worst_idx = -1;
        int max_diff = -1;
        
        for (int j = 0; j < n_partitions; j++) {
            if (partitions[j] >= processes[i]) {
                if (partitions[j] > max_diff) {
                    max_diff = partitions[j];
                    worst_idx = j;
                }
            }
        }
        
        if (worst_idx != -1) {
            allocation[i] = worst_idx;
            partitions[worst_idx] -= processes[i];
        }
    }
    
    // Print allocation results
    printf("\nWorst Fit Allocation:\n");
    for (int i = 0; i < n_processes; i++) {
        printf("Process %d: ", i + 1);
        if (allocation[i] != -1)
            printf("Allocated to Partition %d\n", allocation[i] + 1);
        else
            printf("Not Allocated\n");
    }
}

int main() {
    // int partitions[] = {200, 400, 600, 800, 300, 250};
    // int processes[] = {357, 210, 468, 491};
    int partitions[] = {150, 350};
    int processes[] = {300, 25, 125, 50};
    int n_partitions = sizeof(partitions) / sizeof(partitions[0]);
    int n_processes = sizeof(processes) / sizeof(processes[0]);
    
    int temp_partitions[MAX_PARTITIONS];
    
    // First Fit
    for (int i = 0; i < n_partitions; i++)
        temp_partitions[i] = partitions[i];
    firstFit(temp_partitions, n_partitions, processes, n_processes);
    
    // Best Fit
    for (int i = 0; i < n_partitions; i++)
        temp_partitions[i] = partitions[i];
    bestFit(temp_partitions, n_partitions, processes, n_processes);
    
    // Worst Fit
    for (int i = 0; i < n_partitions; i++)
        temp_partitions[i] = partitions[i];
    worstFit(temp_partitions, n_partitions, processes, n_processes);
    
    return 0;
}
