#include <stdio.h>
#define MAX_PROCESSES 10
#define MAX_FRAMES 50

typedef struct {
    int framesAllocated;  
    int priority;        // (for priority-based allocation)
    int requiredFrames;  // (for proportional allocation)
} Process;

void displayAllocation(Process processes[], int processCount) {
    for (int i = 0; i < processCount; i++) {
        printf("Process %d: Frames Allocated = %d\n", i + 1, processes[i].framesAllocated);
    }
}

// Equal Frame Allocation
void equalFrameAllocation(Process processes[], int processCount, int totalFrames) {
    int framesPerProcess = totalFrames / processCount;
    for (int i = 0; i < processCount; i++) {
        processes[i].framesAllocated = framesPerProcess;
    }
    printf("\nEqual Frame Allocation:\n");
    displayAllocation(processes, processCount);
}

// Proportional Frame Allocation
void proportionalFrameAllocation(Process processes[], int processCount, int totalFrames) {
    int totalRequiredFrames = 0;
    for (int i = 0; i < processCount; i++) {
        totalRequiredFrames += processes[i].requiredFrames;
    }
    for (int i = 0; i < processCount; i++) {
        processes[i].framesAllocated = (processes[i].requiredFrames * totalFrames) / totalRequiredFrames;
    }
    printf("\nProportional Frame Allocation:\n");
    displayAllocation(processes, processCount);
}

// Priority-Based Frame Allocation
// Frames Allocated to Process=(Process Priority / Total Priority) × Total Frames

void priorityBasedFrameAllocation(Process processes[], int processCount, int totalFrames) {
    int totalPriority = 0;
    for (int i = 0; i < processCount; i++) {
        totalPriority += processes[i].priority;
    }
    for (int i = 0; i < processCount; i++) {
        processes[i].framesAllocated = (processes[i].priority * totalFrames) / totalPriority;
    }
    printf("\nPriority-Based Frame Allocation:\n");
    displayAllocation(processes, processCount);
}

int main() {
    int totalFrames = 30; 
    int processCount = 4; 

    Process processes[MAX_PROCESSES] = {
        {0, 3, 8},  // framesAllocated, priority, requiredFrames
        {0, 2, 10},
        {0, 5, 6},
        {0, 1, 6}
    };

    printf("Comparative Analysis of Frame Allocation Techniques\n");

    equalFrameAllocation(processes, processCount, totalFrames);
    proportionalFrameAllocation(processes, processCount, totalFrames);
    priorityBasedFrameAllocation(processes, processCount, totalFrames);

    return 0;
}
