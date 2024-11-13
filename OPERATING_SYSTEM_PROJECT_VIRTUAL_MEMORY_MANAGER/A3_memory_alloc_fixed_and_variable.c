#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_PARTITIONS 5
#define MAX_PROCESSES 4

void displayAllocation(int allocation[], int processes[], int partitions[], int n_processes) {
    for (int i = 0; i < n_processes; i++) {
        printf("Process %d (%d): ", i + 1, processes[i]);
        if (allocation[i] != -1) {
            printf("Allocated to Partition %d (%d)\n", 
                   allocation[i] + 1, 
                   partitions[allocation[i]]);
        } else {
            printf("Not Allocated\n");
        }
    }
}

// Fixed Size Partitioning Algorithms
void firstFitFixed(int partitions[], int n_partitions, int processes[], int n_processes) {
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
    
    printf("\nFirst Fit (Fixed) Allocation:\n");
    displayAllocation(allocation, processes, partitions, n_processes);
}

void nextFitFixed(int partitions[], int n_partitions, int processes[], int n_processes) {
    bool isAllocated[MAX_PARTITIONS] = {false};
    int allocation[MAX_PROCESSES];
    int lastAllocated = -1;
    
    for (int i = 0; i < n_processes; i++)
        allocation[i] = -1;
    
    for (int i = 0; i < n_processes; i++) {
        int j = (lastAllocated + 1) % n_partitions;
        int count = 0;
        
        while (count < n_partitions) {
            if (!isAllocated[j] && partitions[j] >= processes[i]) {
                allocation[i] = j;
                isAllocated[j] = true;
                lastAllocated = j;
                break;
            }
            j = (j + 1) % n_partitions;
            count++;
        }
    }
    
    printf("\nNext Fit (Fixed) Allocation:\n");
    displayAllocation(allocation, processes, partitions, n_processes);
}

void bestFitFixed(int partitions[], int n_partitions, int processes[], int n_processes) {
    bool isAllocated[MAX_PARTITIONS] = {false};
    int allocation[MAX_PROCESSES];
    
    for (int i = 0; i < n_processes; i++)
        allocation[i] = -1;
    
    for (int i = 0; i < n_processes; i++) {
        int bestIdx = -1;
        int minDiff = INT_MAX;
        
        for (int j = 0; j < n_partitions; j++) {
            if (!isAllocated[j] && partitions[j] >= processes[i]) {
                if (partitions[j] - processes[i] < minDiff) {
                    minDiff = partitions[j] - processes[i];
                    bestIdx = j;
                }
            }
        }
        
        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            isAllocated[bestIdx] = true;
        }
    }
    
    printf("\nBest Fit (Fixed) Allocation:\n");
    displayAllocation(allocation, processes, partitions, n_processes);
}

void worstFitFixed(int partitions[], int n_partitions, int processes[], int n_processes) {
    bool isAllocated[MAX_PARTITIONS] = {false};
    int allocation[MAX_PROCESSES];
    
    for (int i = 0; i < n_processes; i++)
        allocation[i] = -1;
    
    for (int i = 0; i < n_processes; i++) {
        int worstIdx = -1;
        int maxDiff = -1;
        
        for (int j = 0; j < n_partitions; j++) {
            if (!isAllocated[j] && partitions[j] >= processes[i]) {
                if (partitions[j] > maxDiff) {
                    maxDiff = partitions[j];
                    worstIdx = j;
                }
            }
        }
        
        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            isAllocated[worstIdx] = true;
        }
    }
    
    printf("\nWorst Fit (Fixed) Allocation:\n");
    displayAllocation(allocation, processes, partitions, n_processes);
}

// Variable Size Partitioning Algorithms
void firstFitVariable(int partitions[], int n_partitions, int processes[], int n_processes) {
    int allocation[MAX_PROCESSES];
    int remainingSpace[MAX_PARTITIONS];
    
    for (int i = 0; i < n_partitions; i++)
        remainingSpace[i] = partitions[i];
    
    for (int i = 0; i < n_processes; i++)
        allocation[i] = -1;
    
    for (int i = 0; i < n_processes; i++) {
        for (int j = 0; j < n_partitions; j++) {
            if (remainingSpace[j] >= processes[i]) {
                allocation[i] = j;
                remainingSpace[j] -= processes[i];
                break;
            }
        }
    }
    
    printf("\nFirst Fit (Variable) Allocation:\n");
    displayAllocation(allocation, processes, partitions, n_processes);
}

void nextFitVariable(int partitions[], int n_partitions, int processes[], int n_processes) {
    int allocation[MAX_PROCESSES];
    int remainingSpace[MAX_PARTITIONS];
    int lastAllocated = -1;
    
    for (int i = 0; i < n_partitions; i++)
        remainingSpace[i] = partitions[i];
    
    for (int i = 0; i < n_processes; i++)
        allocation[i] = -1;
    
    for (int i = 0; i < n_processes; i++) {
        int j = (lastAllocated + 1) % n_partitions;
        int count = 0;
        
        while (count < n_partitions) {
            if (remainingSpace[j] >= processes[i]) {
                allocation[i] = j;
                remainingSpace[j] -= processes[i];
                lastAllocated = j;
                break;
            }
            j = (j + 1) % n_partitions;
            count++;
        }
    }
    
    printf("\nNext Fit (Variable) Allocation:\n");
    displayAllocation(allocation, processes, partitions, n_processes);
}

void bestFitVariable(int partitions[], int n_partitions, int processes[], int n_processes) {
    int allocation[MAX_PROCESSES];
    int remainingSpace[MAX_PARTITIONS];
    
    for (int i = 0; i < n_partitions; i++)
        remainingSpace[i] = partitions[i];
    
    for (int i = 0; i < n_processes; i++)
        allocation[i] = -1;
    
    for (int i = 0; i < n_processes; i++) {
        int bestIdx = -1;
        int minDiff = INT_MAX;
        
        for (int j = 0; j < n_partitions; j++) {
            if (remainingSpace[j] >= processes[i]) {
                if (remainingSpace[j] - processes[i] < minDiff) {
                    minDiff = remainingSpace[j] - processes[i];
                    bestIdx = j;
                }
            }
        }
        
        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            remainingSpace[bestIdx] -= processes[i];
        }
    }
    
    printf("\nBest Fit (Variable) Allocation:\n");
    displayAllocation(allocation, processes, partitions, n_processes);
}

void worstFitVariable(int partitions[], int n_partitions, int processes[], int n_processes) {
    int allocation[MAX_PROCESSES];
    int remainingSpace[MAX_PARTITIONS];
    
    for (int i = 0; i < n_partitions; i++)
        remainingSpace[i] = partitions[i];
    
    for (int i = 0; i < n_processes; i++)
        allocation[i] = -1;
    
    for (int i = 0; i < n_processes; i++) {
        int worstIdx = -1;
        int maxDiff = -1;
        
        for (int j = 0; j < n_partitions; j++) {
            if (remainingSpace[j] >= processes[i]) {
                if (remainingSpace[j] > maxDiff) {
                    maxDiff = remainingSpace[j];
                    worstIdx = j;
                }
            }
        }
        
        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            remainingSpace[worstIdx] -= processes[i];
        }
    }
    
    printf("\nWorst Fit (Variable) Allocation:\n");
    displayAllocation(allocation, processes, partitions, n_processes);
}

int main() {
    int partitions[] = {150, 350};
    int processes[] = {300, 25, 125, 50};
    int n_partitions = sizeof(partitions) / sizeof(partitions[0]);
    int n_processes = sizeof(processes) / sizeof(processes[0]);
    
    int tempPartitionsFixed[MAX_PARTITIONS];
    int tempPartitionsVariable[MAX_PARTITIONS];
    
    printf("Fixed Size Partitioning:\n");
    for (int i = 0; i < n_partitions; i++)
        tempPartitionsFixed[i] = partitions[i];
    firstFitFixed(tempPartitionsFixed, n_partitions, processes, n_processes);
    
    for (int i = 0; i < n_partitions; i++)
        tempPartitionsFixed[i] = partitions[i];
    nextFitFixed(tempPartitionsFixed, n_partitions, processes, n_processes);
    
    for (int i = 0; i < n_partitions; i++)
        tempPartitionsFixed[i] = partitions[i];
    bestFitFixed(tempPartitionsFixed, n_partitions, processes, n_processes);
    
    for (int i = 0; i < n_partitions; i++)
        tempPartitionsFixed[i] = partitions[i];
    worstFitFixed(tempPartitionsFixed, n_partitions, processes, n_processes);
    
    printf("\nVariable Size Partitioning:\n");
    for (int i = 0; i < n_partitions; i++)
        tempPartitionsVariable[i] = partitions[i];
    firstFitVariable(tempPartitionsVariable, n_partitions, processes, n_processes);
    
    for (int i = 0; i < n_partitions; i++)
        tempPartitionsVariable[i] = partitions[i];
    nextFitVariable(tempPartitionsVariable, n_partitions, processes, n_processes);
    
    for (int i = 0; i < n_partitions; i++)
        tempPartitionsVariable[i] = partitions[i];
    bestFitVariable(tempPartitionsVariable, n_partitions, processes, n_processes);
    
    for (int i = 0; i < n_partitions; i++)
        tempPartitionsVariable[i] = partitions[i];
    worstFitVariable(tempPartitionsVariable, n_partitions, processes, n_processes);
    
    return 0;
}

/*
┌─[user@parrot]─[~/Desktop]
└──╼ $./code
Fixed Size Partitioning:

First Fit (Fixed) Allocation:
Process 1 (300): Allocated to Partition 2 (350)
Process 2 (25): Allocated to Partition 1 (150)
Process 3 (125): Not Allocated
Process 4 (50): Not Allocated

Next Fit (Fixed) Allocation:
Process 1 (300): Allocated to Partition 2 (350)
Process 2 (25): Allocated to Partition 1 (150)
Process 3 (125): Not Allocated
Process 4 (50): Not Allocated

Best Fit (Fixed) Allocation:
Process 1 (300): Allocated to Partition 2 (350)
Process 2 (25): Allocated to Partition 1 (150)
Process 3 (125): Not Allocated
Process 4 (50): Not Allocated

Worst Fit (Fixed) Allocation:
Process 1 (300): Allocated to Partition 2 (350)
Process 2 (25): Allocated to Partition 1 (150)
Process 3 (125): Not Allocated
Process 4 (50): Not Allocated

Variable Size Partitioning:

First Fit (Variable) Allocation:
Process 1 (300): Allocated to Partition 2 (350)
Process 2 (25): Allocated to Partition 1 (150)
Process 3 (125): Allocated to Partition 1 (150)
Process 4 (50): Allocated to Partition 2 (350)

Next Fit (Variable) Allocation:
Process 1 (300): Allocated to Partition 2 (350)
Process 2 (25): Allocated to Partition 1 (150)
Process 3 (125): Allocated to Partition 1 (150)
Process 4 (50): Allocated to Partition 2 (350)

Best Fit (Variable) Allocation:
Process 1 (300): Allocated to Partition 2 (350)
Process 2 (25): Allocated to Partition 2 (350)
Process 3 (125): Allocated to Partition 1 (150)
Process 4 (50): Not Allocated

Worst Fit (Variable) Allocation:
Process 1 (300): Allocated to Partition 2 (350)
Process 2 (25): Allocated to Partition 1 (150)
Process 3 (125): Allocated to Partition 1 (150)
Process 4 (50): Allocated to Partition 2 (350)
*/