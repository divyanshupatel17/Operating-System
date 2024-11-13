#include <stdio.h>
#include <math.h>
#include <stdbool.h>

void displayAllocation(int process_size, int process_num, int partition_num) {
    if (partition_num != -1)
        printf("Process %d (%d): Allocated to Partition %d\n", 
               process_num + 1, process_size, partition_num + 1);
    else
        printf("Process %d (%d): Not Allocated\n", process_num + 1, process_size);
}

int getNextPowerOf2(int n) {
    return pow(2, ceil(log2(n)));
}

void buddySystem(int partitions[], int n_partitions, int processes[], int n_processes) {
    int temp_partitions[n_partitions];
    for(int i = 0; i < n_partitions; i++) 
        temp_partitions[i] = partitions[i];
    
    printf("\nBuddy System Allocation:\n");
    for(int i = 0; i < n_processes; i++) {
        int required_size = getNextPowerOf2(processes[i]);
        int allocated = -1;
        
        for(int j = 0; j < n_partitions; j++) {
            if(temp_partitions[j] >= required_size) {
                while(temp_partitions[j] > required_size && 
                      (temp_partitions[j]/2) >= required_size) {
                    temp_partitions[j] /= 2;
                }
                
                if(temp_partitions[j] >= required_size) {
                    allocated = j;
                    temp_partitions[j] -= required_size;
                    break;
                }
            }
        }
        displayAllocation(processes[i], i, allocated);
    }
}

int main() {
    // powers of 2 for buddy system
    int partitions[] = {128, 512, 256, 256, 512};
    int processes[] = {212, 417, 112, 426};
    int n_partitions = sizeof(partitions) / sizeof(partitions[0]);
    int n_processes = sizeof(processes) / sizeof(processes[0]);

    buddySystem(partitions, n_partitions, processes, n_processes);

    return 0;
}

/*
┌─[user@parrot]─[~/Desktop]
└──╼ $./code

Buddy System Allocation:
Process 1 (212): Allocated to Partition 2
Process 2 (417): Allocated to Partition 5
Process 3 (112): Allocated to Partition 1
Process 4 (426): Not Allocated

*/