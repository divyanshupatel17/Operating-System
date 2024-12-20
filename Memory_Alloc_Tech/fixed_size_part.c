// FIXED SIZE PARTINING

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_PARTITIONS 5
#define MAX_PROCESSES 4

void firstFit(int partitions[], int n_partitions, int processes[], int n_processes) {
    bool isAllocated[MAX_PARTITIONS] = {false};
    int allocation[MAX_PROCESSES];
    
    for (int i = 0; i < n_processes; i++)
        allocation[i] = -1;
    
    for (int i = 0; i < n_processes; i++) {
        for (int j = 0; j < n_partitions; j++) {
            if (!isAllocated[j] && partitions[j] >= processes[i]) {
                allocation[i] = j;
                isAllocated[j] = true;
                break;
            }
        }
    }
    
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
    bool isAllocated[MAX_PARTITIONS] = {false};
    int allocation[MAX_PROCESSES];
    
    for (int i = 0; i < n_processes; i++)
        allocation[i] = -1;
    
    for (int i = 0; i < n_processes; i++) {
        int best_idx = -1;
        int min_diff = INT_MAX;
        
        for (int j = 0; j < n_partitions; j++) {
            if (!isAllocated[j] && partitions[j] >= processes[i]) {
                if (partitions[j] - processes[i] < min_diff) {
                    min_diff = partitions[j] - processes[i];
                    best_idx = j;
                }
            }
        }
        
        if (best_idx != -1) {
            allocation[i] = best_idx;
            isAllocated[best_idx] = true;
        }
    }
    
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
    bool isAllocated[MAX_PARTITIONS] = {false};
    int allocation[MAX_PROCESSES];
    
    for (int i = 0; i < n_processes; i++)
        allocation[i] = -1;
    
    for (int i = 0; i < n_processes; i++) {
        int worst_idx = -1;
        int max_diff = -1;
        
        for (int j = 0; j < n_partitions; j++) {
            if (!isAllocated[j] && partitions[j] >= processes[i]) {
                if (partitions[j] > max_diff) {
                    max_diff = partitions[j];
                    worst_idx = j;
                }
            }
        }
        
        if (worst_idx != -1) {
            allocation[i] = worst_idx;
            isAllocated[worst_idx] = true;
        }
    }
    
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
    int partitions[] = {150, 350};
    int processes[] = {300, 25, 125, 50};
    int n_partitions = sizeof(partitions) / sizeof(partitions[0]);
    int n_processes = sizeof(processes) / sizeof(processes[0]);
    
    // First Fit
    firstFit(partitions, n_partitions, processes, n_processes);
    
    // Best Fit
    bestFit(partitions, n_partitions, processes, n_processes);
    
    // Worst Fit
    worstFit(partitions, n_partitions, processes, n_processes);
    
    return 0;
}

// FIXED

First Fit Allocation:
Process 1: Allocated to Partition 2
Process 2: Allocated to Partition 1
Process 3: Not Allocated
Process 4: Not Allocated

Best Fit Allocation:
Process 1: Allocated to Partition 2
Process 2: Allocated to Partition 1
Process 3: Not Allocated
Process 4: Not Allocated

Worst Fit Allocation:
Process 1: Allocated to Partition 2
Process 2: Allocated to Partition 1
Process 3: Not Allocated
Process 4: Not Allocated

// VARIABLE

First Fit Allocation:
Process 1: Allocated to Partition 2
Process 2: Allocated to Partition 1
Process 3: Allocated to Partition 1
Process 4: Allocated to Partition 2

Best Fit Allocation:
Process 1: Allocated to Partition 2
Process 2: Allocated to Partition 2
Process 3: Allocated to Partition 1
Process 4: Not Allocated

Worst Fit Allocation:
Process 1: Allocated to Partition 2
Process 2: Allocated to Partition 1
Process 3: Allocated to Partition 1
Process 4: Allocated to Partition 2
┌─[user@parrot]─[~/Desktop/OSLAB/prac]
└──╼ $

// VARIABLE SIZE PARTINING
