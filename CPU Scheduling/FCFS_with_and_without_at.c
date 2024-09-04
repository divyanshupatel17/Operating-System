#include <stdio.h>
#include <stdlib.h> // For qsort()

#define MAX 20

typedef struct {
    int id;       // Process ID
    int bt;       // Burst Time
    int at;       // Arrival Time
    int ct;       // Completion Time
    int wt;       // Waiting Time
    int tat;      // Turnaround Time
    int rt;       // Response Time
} Process;

int compareArrivalTime(const void *a, const void *b) {
    return ((Process*)a)->at - ((Process*)b)->at;
}

void fcfs(Process processes[], int n) {
    float wtavg = 0, tatavg = 0;
    int currentTime = 0;  // Keep track of the current time
    int idleTime = 0;     // Track total idle time
    
    // Sort processes by arrival time
    qsort(processes, n, sizeof(Process), compareArrivalTime);
    
    // Calculate waiting time, turnaround time, completion time, and response time
    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].at) {
            idleTime += processes[i].at - currentTime;  // Calculate idle time
            currentTime = processes[i].at;  // Jump to the arrival time of the current process
        }
        processes[i].rt = currentTime - processes[i].at;
        processes[i].wt = processes[i].rt;
        processes[i].tat = processes[i].wt + processes[i].bt;
        processes[i].ct = currentTime + processes[i].bt;
        currentTime += processes[i].bt;  // Update the current time after process execution
    }
    
    // Calculate average waiting time and turnaround time
    for (int i = 0; i < n; i++) {
        wtavg += processes[i].wt;
        tatavg += processes[i].tat;
    }

    // Display the results
    printf("\nPROCESS ID\tBURST TIME\tARRIVAL TIME\tCOMPLETION TIME\tWAITING TIME\tTURNAROUND TIME\tRESPONSE TIME\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].bt, processes[i].at, processes[i].ct, processes[i].wt, processes[i].tat, processes[i].rt);
    }
    
    printf("\nAverage Waiting Time: %.2f", wtavg / n);
    printf("\nAverage Turnaround Time: %.2f", tatavg / n);
    printf("\nTotal Idle Time: %d\n", idleTime);

    // Display Gantt Chart
    printf("\nGantt Chart:\n");

    // Print the top border
    printf("Timeline: ");
    printf("0");
    for (int i = 0; i < n; i++) {
        printf("-----");
    }
    printf("\n");

    // Print process identifiers
    printf("Process: ");
    for (int i = 0; i < n; i++) {
        printf("P%d\t", processes[i].id);
    }
    printf("\n");

    // Print the bottom border
    printf("Timeline: ");
    printf("0");
    for (int i = 0; i < n; i++) {
        printf("-----");
    }
    printf("\n");

    // Detailed Gantt chart with idle times
    printf("\nDetailed Gantt Chart:\n");
    int lastTime = 0;
    for (int i = 0; i < n; i++) {
        if (processes[i].at > lastTime) {
            printf("Idle (%d-%d)\t", lastTime, processes[i].at);
        }
        printf("P%d (%d-%d)\t", processes[i].id, processes[i].ct - processes[i].bt, processes[i].ct);
        lastTime = processes[i].ct;
    }
    if (lastTime < currentTime) {
        printf("Idle (%d-%d)\t", lastTime, currentTime);
    }
    printf("\n");
}

int main() {
    int n;
    Process processes[MAX];
    int useAT;  // Flag to decide whether to use arrival time or not

    // Input number of processes
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    // Check if number of processes is valid
    if (n <= 0 || n > MAX) {
        printf("Invalid number of processes.\n");
        return 1;
    }
    
    // Ask whether to include arrival time
    printf("Include Arrival Time (0 for No, 1 for Yes): ");
    scanf("%d", &useAT);

    // Input process details
    for (int i = 0; i < n; i++) {
        printf("Enter Process ID for Process %d: ", i);
        scanf("%d", &processes[i].id);
        
        if (useAT) {
            printf("Enter Arrival Time for Process %d: ", i);
            scanf("%d", &processes[i].at);
        } else {
            processes[i].at = 0;  // Initialize arrival time to 0 if not used
        }
        
        printf("Enter Burst Time for Process %d: ", i);
        scanf("%d", &processes[i].bt);
    }

    // Call the FCFS scheduling function
    fcfs(processes, n);

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
Enter Arrival Time for Process 0: 2
Enter Burst Time for Process 0: 2
Enter Process ID for Process 1: 2
Enter Arrival Time for Process 1: 0
Enter Burst Time for Process 1: 1
Enter Process ID for Process 2: 3
Enter Arrival Time for Process 2: 2
Enter Burst Time for Process 2: 3
Enter Process ID for Process 3: 4
Enter Arrival Time for Process 3: 3
Enter Burst Time for Process 3: 5
Enter Process ID for Process 4: 5
Enter Arrival Time for Process 4: 4
Enter Burst Time for Process 4: 4

PROCESS ID      BURST TIME      ARRIVAL TIME    COMPLETION TIME WAITING TIME    TURNAROUND TIME RESPONSE TIME
2               1               0               1               0               1               0
1               2               2               4               0               2               0
3               3               2               7               2               5               2
4               5               3               12              4               9               4
5               4               4               16              8               12              8

Average Waiting Time: 2.80
Average Turnaround Time: 5.80
Total Idle Time: 1

Gantt Chart:
Timeline: 0-------------------------
Process: P2     P1      P3      P4      P5
Timeline: 0-------------------------

Detailed Gantt Chart:
P2 (0-1)        Idle (1-2)      P1 (2-4)        P3 (4-7)        P4 (7-12)       P5 (12-16)

 */