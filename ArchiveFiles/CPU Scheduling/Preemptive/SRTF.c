// NOTE : srtf behaves like sjf if all process have same arrivial time or AT=0 for all

#include <stdio.h>
#include <stdlib.h>

#define MAX 20

// Structure to represent a process
typedef struct {
    int id;             // Process ID
    int bt;             // Burst Time
    int at;             // Arrival Time
    int rt;             // Remaining Time
    int ct;             // Completion Time
    int wt;             // Waiting Time
    int tat;            // Turnaround Time
    int start_time;     // Start time of the process
    int completed;      // Completion status (0 or 1)
} Process;

// Function to compare processes based on arrival time
int compareArrivalTime(const void *a, const void *b) {
    return ((Process*)a)->at - ((Process*)b)->at;
}

// Function to display the ready queue at the current time
void displayReadyQueue(Process processes[], int n, int currentTime) {
    printf("Ready Queue at time %d: ", currentTime);
    for (int i = 0; i < n; i++) {
        if (processes[i].at <= currentTime && processes[i].completed == 0 && processes[i].rt > 0) {
            printf("P%d ", processes[i].id);
        }
    }
    printf("\n");
}

// Function to implement SRTF Scheduling Algorithm
void srtf(Process processes[], int n) {
    float wtavg = 0, tatavg = 0;  // Average waiting time and turnaround time
    int currentTime = 0;          // Current time in the system
    int completedProcesses = 0;   // Counter for completed processes
    int idleTime = 0;             // Total idle time
    
    // Sort processes by arrival time
    qsort(processes, n, sizeof(Process), compareArrivalTime);

    // Initialize remaining time for all processes
    for (int i = 0; i < n; i++) {
        processes[i].rt = processes[i].bt;
        processes[i].completed = 0;
    }

    Process executionOrder[MAX * 2]; // Array to store execution order for Gantt chart
    int executionIndex = 0;          // Index for Gantt chart
    Process idleProcess = {-1, 0, 0, 0, 0, 0, 0, 0, 0}; // Placeholder for idle time
    
    int lastCompletedTime = 0; // Track the last completed process time to handle idle time efficiently

    // Continue until all processes are completed
    while (completedProcesses < n) {
        int minIndex = -1;
        int minRemainingTime = __INT_MAX__; // Initialize with maximum integer value
        
        // Select the process with the minimum remaining time that has arrived and is not completed
        for (int i = 0; i < n; i++) {
            if (processes[i].at <= currentTime && processes[i].completed == 0 && processes[i].rt > 0) {
                if (processes[i].rt < minRemainingTime) {
                    minRemainingTime = processes[i].rt;
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
                idleProcess.start_time = currentTime - 1;
                idleProcess.ct = currentTime;
                executionOrder[executionIndex++] = idleProcess;
            } else {
                executionOrder[executionIndex-1].ct = currentTime;
            }
        } else {
            // Display the ready queue at the current time
            displayReadyQueue(processes, n, currentTime);

            // Update start time for the process (first time it runs)
            if (processes[minIndex].rt == processes[minIndex].bt) {
                processes[minIndex].start_time = currentTime;
            }

            // Execute the process for one unit of time
            processes[minIndex].rt--;
            currentTime++;

            // Store the process in the execution order for the Gantt chart
            executionOrder[executionIndex] = processes[minIndex];
            executionOrder[executionIndex].ct = currentTime;
            executionIndex++;

            // If process is completed, calculate its metrics
            if (processes[minIndex].rt == 0) {
                processes[minIndex].completed = 1;
                completedProcesses++;
                processes[minIndex].ct = currentTime;
                processes[minIndex].tat = processes[minIndex].ct - processes[minIndex].at;
                processes[minIndex].wt = processes[minIndex].tat - processes[minIndex].bt;

                // Track last completed process time
                lastCompletedTime = currentTime;
            }
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
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].bt, processes[i].at, processes[i].ct, processes[i].wt, processes[i].tat, processes[i].start_time - processes[i].at);
    }

    // Display average times and idle time
    printf("\nAverage Waiting Time: %.2f", wtavg / n);
    printf("\nAverage Turnaround Time: %.2f", tatavg / n);
    printf("\nTotal Idle Time: %d\n", idleTime);

    // Display Gantt Chart
    printf("\nGantt Chart:\n");
    printf("0");
    for (int i = 0; i < executionIndex; i++) {
        printf("----");
        printf("%2d", executionOrder[i].ct);
    }
    printf("\n");

    for (int i = 0; i < executionIndex; i++) {
        printf("|");
        int spaces = executionOrder[i].ct - (i == 0 ? 0 : executionOrder[i-1].ct) - 1;
        for (int j = 0; j < spaces / 2; j++) {
            printf(" ");
        }
        if (executionOrder[i].id == -1) {
            printf("Idle");
        } else {
            printf(" P%d  ", executionOrder[i].id);
        }
        for (int j = 0; j < spaces - spaces / 2; j++) {
            printf(" ");
        }
    }
    printf("|\n");

    printf("0");
    for (int i = 0; i < executionIndex; i++) {
        printf("----");
        printf("%2d", executionOrder[i].ct);
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
    srtf(processes, n); // Call the scheduling algorithm

    return 0;
}

/*
                                                                                                                    
┌──(divyanshu㉿kali)-[~/Desktop]
└─$ gcc temp.c -o code
                                                                                                                                                                                                                                            
┌──(divyanshu㉿kali)-[~/Desktop]
└─$ ./code            
Enter the number of processes: 5
Include Arrival Time (0 for No, 1 for Yes): 1
Enter Process ID for Process 0: 1
Enter Arrival Time for Process 0: 10
Enter Burst Time for Process 0: 5
Enter Process ID for Process 1: 2
Enter Arrival Time for Process 1: 8
Enter Burst Time for Process 1: 4
Enter Process ID for Process 2: 3
Enter Arrival Time for Process 2: 12
Enter Burst Time for Process 2: 4
Enter Process ID for Process 3: 4
Enter Arrival Time for Process 3: 3
Enter Burst Time for Process 3: 3
Enter Process ID for Process 4: 5
Enter Arrival Time for Process 4: 15
Enter Burst Time for Process 4: 5


Ready Queue at time 3: P4 
Ready Queue at time 4: P4 
Ready Queue at time 5: P4 
Ready Queue at time 8: P2 
Ready Queue at time 9: P2 
Ready Queue at time 10: P2 P1 
Ready Queue at time 11: P2 P1 
Ready Queue at time 12: P1 P3 
Ready Queue at time 13: P1 P3 
Ready Queue at time 14: P1 P3 
Ready Queue at time 15: P1 P3 P5 
Ready Queue at time 16: P1 P5 
Ready Queue at time 17: P1 P5 
Ready Queue at time 18: P1 P5 
Ready Queue at time 19: P1 P5 
Ready Queue at time 20: P1 P5 
Ready Queue at time 21: P5 
Ready Queue at time 22: P5 
Ready Queue at time 23: P5 
Ready Queue at time 24: P5 
Ready Queue at time 25: P5 

PROCESS ID      BURST TIME      ARRIVAL TIME    COMPLETION TIME WAITING TIME    TURNAROUND TIME RESPONSE TIME
4               3               3               6               0               3               0
2               4               8               12              0               4               0
1               5               10              21              6               11              6
3               4               12              16              0               4               0
5               5               15              26              6               11              6

Average Waiting Time: 2.40
Average Turnaround Time: 6.60
Total Idle Time: 5

Gantt Chart:
0---- 3---- 4---- 5---- 6---- 8---- 9----10----11----12----13----14----15----16----17----18----19----20----21----22----23----24----25----26
| Idle | P4  | P4  | P4  |Idle | P2  | P2  | P2  | P2  | P3  | P3  | P3  | P3  | P1  | P1  | P1  | P1  | P1  | P5  | P5  | P5  | P5  | P5  |
0---- 3---- 4---- 5---- 6---- 8---- 9----10----11----12----13----14----15----16----17----18----19----20----21----22----23----24----25----26

*/