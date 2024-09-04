#include <stdio.h>
#include <stdlib.h>

#define MAX 20

// Structure to represent a process
typedef struct {
    int id;             // Process ID
    int bt;             // Burst Time
    int at;             // Arrival Time
    int ct;             // Completion Time
    int wt;             // Waiting Time
    int tat;            // Turnaround Time
    int rt;             // Response Time
    int completed;      // Completion status (0 or 1)
    int start_time;     // Start time of the process
} Process;

// Function to compare processes based on arrival time
int compareArrivalTime(const void *a, const void *b) {
    return ((Process*)a)->at - ((Process*)b)->at;
}

// Function to compare processes based on burst time
int compareBurstTime(const void *a, const void *b) {
    return ((Process*)a)->bt - ((Process*)b)->bt;
}

// Function to display the ready queue at the current time
void displayReadyQueue(Process processes[], int n, int currentTime) {
    printf("Ready Queue at time %d: ", currentTime);
    for (int i = 0; i < n; i++) {
        if (processes[i].at <= currentTime && processes[i].completed == 0) {
            printf("P%d ", processes[i].id);
        }
    }
    printf("\n");
}

// Function to implement SJF Non-preemptive Scheduling Algorithm
void sjf_non_preemptive(Process processes[], int n) {
    float wtavg = 0, tatavg = 0; // Average waiting time and turnaround time
    int currentTime = 0;         // Current time in the system
    int completedProcesses = 0;  // Counter for completed processes
    int idleTime = 0;            // Total idle time
    
    // Sort processes by arrival time
    qsort(processes, n, sizeof(Process), compareArrivalTime);
    
    // Initialize all processes as not completed
    for (int i = 0; i < n; i++) {
        processes[i].completed = 0;
    }
    
    Process executionOrder[MAX * 2]; // Array to store execution order for Gantt chart
    int executionIndex = 0;          // Index for Gantt chart
    Process idleProcess = {-1, 0, 0, 0, 0, 0, 0, 1, 0}; // Placeholder for idle time
    
    // Continue until all processes are completed
    while (completedProcesses < n) {
        int minIndex = -1;
        int minBurstTime = __INT_MAX__; // Initialize with maximum integer value
        
        // Select the process with the minimum burst time that has arrived and is not completed
        for (int i = 0; i < n; i++) {
            if (processes[i].at <= currentTime && processes[i].completed == 0) {
                if (processes[i].bt < minBurstTime) {
                    minBurstTime = processes[i].bt;
                    minIndex = i;
                }
            }
        }
        
        // If no process is ready, increment idle time
        if (minIndex == -1) {
            idleTime++;
            currentTime++;
            // Track idle time in execution order
            if (executionIndex == 0 || executionOrder[executionIndex-1].id != -1) {
                idleProcess.bt = 1;
                idleProcess.start_time = currentTime - 1;
                idleProcess.ct = currentTime;
                executionOrder[executionIndex++] = idleProcess;
            } else {
                executionOrder[executionIndex-1].bt++;
                executionOrder[executionIndex-1].ct = currentTime;
            }
        } else {
            // Display the ready queue at the current time
            displayReadyQueue(processes, n, currentTime);
            
            // Calculate completion, waiting, and turnaround times for the selected process
            processes[minIndex].start_time = currentTime;
            processes[minIndex].rt = currentTime - processes[minIndex].at;
            processes[minIndex].ct = currentTime + processes[minIndex].bt;
            processes[minIndex].tat = processes[minIndex].ct - processes[minIndex].at;
            processes[minIndex].wt = processes[minIndex].tat - processes[minIndex].bt;
            processes[minIndex].completed = 1;
            
            // Store the process in the execution order for the Gantt chart
            executionOrder[executionIndex++] = processes[minIndex];
            
            // Move current time forward to process completion time
            currentTime = processes[minIndex].ct;
            completedProcesses++;
        }
    }
    
    // Calculate average waiting and turnaround times
    for (int i = 0; i < n; i++) {
        wtavg += processes[i].wt;
        tatavg += processes[i].tat;
    }

    // Display the process details
    printf("\nPROCESS ID\tBURST TIME\tARRIVAL TIME\tCOMPLETION TIME\tWAITING TIME\tTURNAROUND TIME\tRESPONSE TIME\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].bt, processes[i].at, processes[i].ct, processes[i].wt, processes[i].tat, processes[i].rt);
    }
    
    // Display average times and idle time
    printf("\nAverage Waiting Time: %.2f", wtavg / n);
    printf("\nAverage Turnaround Time: %.2f", tatavg / n);
    printf("\nTotal Idle Time: %d\n", idleTime);

    // Display Gantt Chart : FORMAT 1
    printf("\nGantt Chart:\n");
    printf("0");
    for (int i = 0; i < executionIndex; i++) {
        printf("----");
        printf("%d", executionOrder[i].ct);
    }
    printf("\n");

    // Display process execution in Gantt chart format
    for (int i = 0; i < executionIndex; i++) {
        printf("|");
        int spaces = executionOrder[i].bt - 1;
        for (int j = 0; j < spaces / 2; j++) {
            printf(" ");
        }
        if (executionOrder[i].id == -1) {
            printf("Idle");
        } else {
            printf("P%d", executionOrder[i].id);
        }
        for (int j = 0; j < spaces - spaces / 2; j++) {
            printf(" ");
        }
    }
    printf("|\n");

    printf("0");
    for (int i = 0; i < executionIndex; i++) {
        printf("----");
        printf("%d", executionOrder[i].ct);
    }
    printf("\n");

    // Display Gantt Chart : FORMAT 2
    printf("\nDetailed Gantt Chart:\n");
    for (int i = 0; i < executionIndex; i++) {
        if (executionOrder[i].id == -1) {
            printf("Idle (%d-%d)\t", executionOrder[i].start_time, executionOrder[i].ct);
        } else {
            printf("P%d (%d-%d)\t", executionOrder[i].id, executionOrder[i].start_time, executionOrder[i].ct);
        }
    }
    printf("\n");
}

int main() {
    int n;
    Process processes[MAX];
    int useAT;

    // Get the number of processes from the user
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    if (n <= 0 || n > MAX) {
        printf("Invalid number of processes.\n");
        return 1;
    }
    
    // Ask if the user wants to include arrival times
    printf("Include Arrival Time (0 for No, 1 for Yes): ");
    scanf("%d", &useAT);

    // Input process details from the user
    for (int i = 0; i < n; i++) {
        printf("Enter Process ID for Process %d: ", i);
        scanf("%d", &processes[i].id);
        
        if (useAT) {
            printf("Enter Arrival Time for Process %d: ", i);
            scanf("%d", &processes[i].at);
        } else {
            processes[i].at = 0; // Default arrival time to 0 if not used
        }
        
        printf("Enter Burst Time for Process %d: ", i);
        scanf("%d", &processes[i].bt);
    }

    printf("\n\n");
    sjf_non_preemptive(processes, n); // Call the scheduling algorithm

    return 0;
}

/*
┌──(divyanshu㉿kali)-[~/Desktop]
└─$ gcc temp.c -o code
                                                                                                                                                                                                                                            
┌──(divyanshu㉿kali)-[~/Desktop]
└─$ ./code            
Enter the number of processes: 4
Include Arrival Time (0 for No, 1 for Yes): 1
Enter Process ID for Process 0: 1
Enter Arrival Time for Process 0: 1
Enter Burst Time for Process 0: 3
Enter Process ID for Process 1: 2
Enter Arrival Time for Process 1: 2
Enter Burst Time for Process 1: 4
Enter Process ID for Process 2: 3
Enter Arrival Time for Process 2: 1
Enter Burst Time for Process 2: 2
Enter Process ID for Process 3: 4
Enter Arrival Time for Process 3: 4
Enter Burst Time for Process 3: 4


Ready Queue at time 1: P1 P3 
Ready Queue at time 3: P1 P2 
Ready Queue at time 6: P2 P4 
Ready Queue at time 10: P4 

PROCESS ID      BURST TIME      ARRIVAL TIME    COMPLETION TIME WAITING TIME    TURNAROUND TIME RESPONSE TIME
1               3               1               6               2               5               2
3               2               1               3               0               2               0
2               4               2               10              4               8               4
4               4               4               14              6               10              6

Average Waiting Time: 3.00
Average Turnaround Time: 6.25
Total Idle Time: 1

Gantt Chart:
0----1----3----6----10----14
|Idle|P3 | P1 | P2  | P4  |
0----1----3----6----10----14

Detailed Gantt Chart:
Idle (0-1)      P3 (1-3)        P1 (3-6)        P2 (6-10)       P4 (10-14)

 */