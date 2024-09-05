#include <stdio.h>
#include <stdlib.h> // For qsort()

#define MAX 20

typedef struct {
    int id;             // Process ID
    int bt;             // Burst Time
    int at;             // Arrival Time
    int priority;       // Priority (lower value means higher priority)
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

// Function to compare processes based on priority
int comparePriority(const void *a, const void *b) {
    return ((Process*)a)->priority - ((Process*)b)->priority;
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

// Function to display Gantt Chart
void displayGanttChart(Process processes[], int n, Process executionOrder[], int executionIndex) {
    printf("\nGantt Chart:\n");
    printf("0");
    for (int i = 0; i < executionIndex; i++) {
        printf("----");
        printf("%2d", executionOrder[i].ct);
    }
    printf("\n|");
    for (int i = 0; i < executionIndex; i++) {
        if (executionOrder[i].id == -1) {
            printf("Idle |");
        } else {
            printf(" P%d  |", executionOrder[i].id);
        }
    }
    printf("\n0");
    for (int i = 0; i < executionIndex; i++) {
        printf("----");
        printf("%2d", executionOrder[i].ct);
    }
    printf("\n");

    // Display Detailed Gantt Chart
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

// Function to implement non-preemptive Priority Scheduling Algorithm
void priority_non_preemptive(Process processes[], int n) {
    float wtavg = 0, tatavg = 0;    // Average waiting time and turnaround time
    int currentTime = 0;            // Current time in the system
    int completedProcesses = 0;     // Counter for completed processes
    int idleTime = 0;               // Total idle time
    
    // Sort processes by arrival time
    qsort(processes, n, sizeof(Process), compareArrivalTime);
    
    // Initialize all processes as not completed
    for (int i = 0; i < n; i++) {
        processes[i].completed = 0;
    }
    
    Process executionOrder[MAX * 2]; // Array to store execution order for Gantt chart
    int executionIndex = 0;          // Index for Gantt chart
    Process idleProcess = {-1, 0, 0, 0, 0, 0, 0, 0, 1, 0}; // Placeholder for idle time
    
    // Continue until all processes are completed
    while (completedProcesses < n) {
        int highestPriorityIndex = -1;
        int highestPriority = __INT_MAX__; // Initialize with maximum integer value
        
        // Select the process with the highest priority that has arrived and is not completed
        for (int i = 0; i < n; i++) {
            if (processes[i].at <= currentTime && processes[i].completed == 0) {
                if (processes[i].priority < highestPriority) {
                    highestPriority = processes[i].priority;
                    highestPriorityIndex = i;
                }
            }
        }
        
        // If no process is ready and idle time is included
        if (highestPriorityIndex == -1) {
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
        } else { // If a process is ready
            // Display the ready queue at the current time
            displayReadyQueue(processes, n, currentTime);
            
            // Calculate completion, waiting, and turnaround times for the selected process
            processes[highestPriorityIndex].start_time = currentTime;
            processes[highestPriorityIndex].rt = currentTime - processes[highestPriorityIndex].at;
            processes[highestPriorityIndex].ct = currentTime + processes[highestPriorityIndex].bt;
            processes[highestPriorityIndex].tat = processes[highestPriorityIndex].ct - processes[highestPriorityIndex].at;
            processes[highestPriorityIndex].wt = processes[highestPriorityIndex].tat - processes[highestPriorityIndex].bt;
            processes[highestPriorityIndex].completed = 1;
            
            // Store the process in the execution order for the Gantt chart
            executionOrder[executionIndex++] = processes[highestPriorityIndex];
            
            // Move current time forward to process completion time
            currentTime = processes[highestPriorityIndex].ct;
            completedProcesses++;
        }
    }
    
    // Calculate average waiting and turnaround times
    for (int i = 0; i < n; i++) {
        wtavg += processes[i].wt;
        tatavg += processes[i].tat;
    }

    // Display the process details
    printf("\nPROCESS ID\tBURST TIME\tARRIVAL TIME\tPRIORITY\tCOMPLETION TIME\tWAITING TIME\tTURNAROUND TIME\tRESPONSE TIME\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", 
               processes[i].id, processes[i].bt, processes[i].at, processes[i].priority,
               processes[i].ct, processes[i].wt, processes[i].tat, processes[i].rt);
    }
    
    // Display average times and idle time
    printf("\nAverage Waiting Time: %.2f", wtavg / n);
    printf("\nAverage Turnaround Time: %.2f", tatavg / n);
    printf("\nTotal Idle Time: %d\n", idleTime);
    
    // Display Gantt Chart
    displayGanttChart(processes, n, executionOrder, executionIndex);
}

int main() {
    int n, useAT;
    Process processes[MAX];

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
        
        printf("Enter Priority for Process %d (lower value means higher priority): ", i);
        scanf("%d", &processes[i].priority);
    }

    printf("\n\n");
    priority_non_preemptive(processes, n); // Call the scheduling algorithm

    return 0;
}

/*
┌──(divyanshu㉿kali)-[~/Desktop]
└─$ gcc temp.c -o code
                                                                                                                    
┌──(divyanshu㉿kali)-[~/Desktop]
└─$ ./code            
Enter the number of processes: 7
Include Arrival Time (0 for No, 1 for Yes): 1
Enter Process ID for Process 0: 1
Enter Arrival Time for Process 0: 0
Enter Burst Time for Process 0: 3
Enter Priority for Process 0 (lower value means higher priority): 2
Enter Process ID for Process 1: 2
Enter Arrival Time for Process 1: 2
Enter Burst Time for Process 1: 5
Enter Priority for Process 1 (lower value means higher priority): 6
Enter Process ID for Process 2: 3
Enter Arrival Time for Process 2: 1
Enter Burst Time for Process 2: 4
Enter Priority for Process 2 (lower value means higher priority): 3
Enter Process ID for Process 3: 4
Enter Arrival Time for Process 3: 4
Enter Burst Time for Process 3: 2
Enter Priority for Process 3 (lower value means higher priority): 5
Enter Process ID for Process 4: 5
Enter Arrival Time for Process 4: 6
Enter Burst Time for Process 4: 9
Enter Priority for Process 4 (lower value means higher priority): 7
Enter Process ID for Process 5: 6
Enter Arrival Time for Process 5: 5
Enter Burst Time for Process 5: 4
Enter Priority for Process 5 (lower value means higher priority): 4
Enter Process ID for Process 6: 7
Enter Arrival Time for Process 6: 7
Enter Burst Time for Process 6: 10
Enter Priority for Process 6 (lower value means higher priority): 10


Ready Queue at time 0: P1 
Ready Queue at time 3: P3 P2 
Ready Queue at time 7: P2 P4 P6 P5 P7 
Ready Queue at time 11: P2 P4 P5 P7 
Ready Queue at time 13: P2 P5 P7 
Ready Queue at time 18: P5 P7 
Ready Queue at time 27: P7 

PROCESS ID      BURST TIME      ARRIVAL TIME    PRIORITY        COMPLETION TIME WAITING TIME    TURNAROUND TIME RESPONSE TIME
1               3               0               2               3               0               3               0
3               4               1               3               7               2               6               2
2               5               2               6               18              11              16              11
4               2               4               5               13              7               9               7
6               4               5               4               11              2               6               2
5               9               6               7               27              12              21              12
7               10              7               10              37              20              30              20

Average Waiting Time: 7.71
Average Turnaround Time: 13.00
Total Idle Time: 0

Gantt Chart:
0---- 3---- 7----11----13----18----27----37
| P1  | P3  | P6  | P4  | P2  | P5  | P7  |
0---- 3---- 7----11----13----18----27----37

Detailed Gantt Chart:
P1 (0-3)        P3 (3-7)        P6 (7-11)       P4 (11-13)      P2 (13-18)      P5 (18-27)      P7 (27-37)

 */

 // EASY CODE
#include <stdio.h>

typedef struct {
    int id;             // Process ID
    int bt;             // Burst Time
    int at;             // Arrival Time
    int priority;       // Priority (lower value means higher priority)
    int ct;             // Completion Time
    int wt;             // Waiting Time
    int tat;            // Turnaround Time
    int rt;             // Response Time
} Process;

void findWaitingTime(Process proc[], int n) {
    proc[0].wt = 0; // Waiting time for first process is 0
    for (int i = 1; i < n; i++) {
        proc[i].wt = proc[i-1].ct - proc[i].at;
        if (proc[i].wt < 0) proc[i].wt = 0;
    }
}

void findTurnAroundTime(Process proc[], int n) {
    for (int i = 0; i < n; i++) {
        proc[i].tat = proc[i].bt + proc[i].wt;
    }
}

void findCompletionTime(Process proc[], int n) {
    proc[0].ct = proc[0].at + proc[0].bt;
    for (int i = 1; i < n; i++) {
        if (proc[i].at > proc[i-1].ct) {
            proc[i].ct = proc[i].at + proc[i].bt;
        } else {
            proc[i].ct = proc[i-1].ct + proc[i].bt;
        }
    }
}

void printReadyQueue(Process proc[], int n, int currentTime) {
    printf("Ready Queue at time %d: ", currentTime);
    for (int i = 0; i < n; i++) {
        if (proc[i].at <= currentTime && proc[i].ct == 0) {
            printf("P%d ", proc[i].id);
        }
    }
    printf("\n");
}

void priorityScheduling(Process proc[], int n) {
    // Sort processes by priority (lower value means higher priority)
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (proc[i].priority > proc[j].priority) {
                Process temp = proc[i];
                proc[i] = proc[j];
                proc[j] = temp;
            }
        }
    }

    int currentTime = 0;
    int totalIdleTime = 0;
    for (int i = 0; i < n; i++) {
        if (proc[i].at > currentTime) {
            totalIdleTime += proc[i].at - currentTime;
            currentTime = proc[i].at;
        }
        printReadyQueue(proc, n, currentTime);
        proc[i].ct = currentTime + proc[i].bt;
        currentTime = proc[i].ct;
    }

    findWaitingTime(proc, n);
    findTurnAroundTime(proc, n);

    int total_wt = 0, total_tat = 0;
    printf("ID\tBT\tAT\tPriority\tCT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        total_wt += proc[i].wt;
        total_tat += proc[i].tat;
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\t%d\n", proc[i].id, proc[i].bt, proc[i].at, proc[i].priority, proc[i].ct, proc[i].wt, proc[i].tat);
    }

    printf("Average Waiting Time: %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_tat / n);
    printf("Total Idle Time: %d\n", totalIdleTime);

    // Gantt Chart
    printf("\nGantt Chart:\n");
    int lastCompletionTime = 0;
    for (int i = 0; i < n; i++) {
        if (proc[i].at > lastCompletionTime) {
            printf("| Idle ");
            lastCompletionTime = proc[i].at;
        }
        printf("| P%d ", proc[i].id);
        lastCompletionTime = proc[i].ct;
    }
    printf("|\n");
    lastCompletionTime = 0;
    for (int i = 0; i < n; i++) {
        if (proc[i].at > lastCompletionTime) {
            printf("%d\t", lastCompletionTime);
            lastCompletionTime = proc[i].at;
        }
        printf("%d\t", proc[i].ct - proc[i].bt);
        lastCompletionTime = proc[i].ct;
    }
    printf("%d\n", proc[n-1].ct);
}


int main() {
    Process proc[] = {{1, 3, 0, 2}, {2, 5, 2, 6}, {3, 4, 1, 3},{4,2,4,5},{5,9,6,7},{6,4,5,4},{7,10,7,10}};
    int n = sizeof(proc) / sizeof(proc[0]);

    priorityScheduling(proc, n);

    return 0;
}

/*
Ready Queue at time 0: P1 
Ready Queue at time 3: P3 P2 
Ready Queue at time 7: P6 P4 P2 P5 P7 
Ready Queue at time 11: P4 P2 P5 P7 
Ready Queue at time 13: P2 P5 P7 
Ready Queue at time 18: P5 P7 
Ready Queue at time 27: P7 
ID	BT	AT	Priority	CT	WT	TAT
1	3	0	2		3	0	3
3	4	1	3		7	2	6
6	4	5	4		11	2	6
4	2	4	5		13	7	9
2	5	2	6		18	11	16
5	9	6	7		27	12	21
7	10	7	10		37	20	30
Average Waiting Time: 7.71
Average Turnaround Time: 13.00
Total Idle Time: 0

Gantt Chart:
| P1 | P3 | P6 | P4 | P2 | P5 | P7 |
0	3	7	11	13	18	27	37


=== Code Execution Successful ===
*/