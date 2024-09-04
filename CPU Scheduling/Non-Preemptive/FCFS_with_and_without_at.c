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
    int completed; // Flag to indicate if the process is completed
} Process;

int compareArrivalTime(const void *a, const void *b) {
    return ((Process*)a)->at - ((Process*)b)->at;
}

void displayReadyQueue(Process processes[], int n, int currentTime) {
    printf("Ready Queue at time %d: ", currentTime);
    for (int i = 0; i < n; i++) {
        if (processes[i].at <= currentTime && processes[i].completed == 0) {
            printf("P%d ", processes[i].id);
        }
    }
    printf("\n");
}

void displayGanttChart(Process processes[], int n) {
    int currentTime = 0;

    printf("\nGantt Chart:\n");

    // Print top timeline
    printf("%2d", currentTime);
    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].at) {
            printf("----");
            printf("%2d", processes[i].at);
            currentTime = processes[i].at;
        }
        printf("----");
        currentTime += processes[i].bt;
        printf("%2d", currentTime);
    }
    printf("\n");

    // Print process IDs (and Idle if applicable)
    currentTime = 0;
    printf(" |");
    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].at) {
            printf("Idle |");
            currentTime = processes[i].at;
        }
        // Print process ID
        for (int j = 0; j < processes[i].bt; j++) {
            if (j == 0) {
                printf(" P%d  ", processes[i].id);
            } 
        }
        printf("|");
        currentTime += processes[i].bt;
    }
    printf("\n");

    // Print bottom timeline
    currentTime = 0;
    printf("%2d", currentTime);
    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].at) {
            printf("----");
            printf("%2d", processes[i].at);
            currentTime = processes[i].at;
        }
        printf("----");
        currentTime += processes[i].bt;
        printf("%2d", currentTime);
    }
    printf("\n");
}


void fcfs(Process processes[], int n) {
    float wtavg = 0, tatavg = 0;
    int currentTime = 0;  // Keep track of the current time
    int idleTime = 0;     // Track total idle time
    
    // Sort processes by arrival time
    qsort(processes, n, sizeof(Process), compareArrivalTime);
    
    // Initialize the completed flag to 0 (not completed)
    for (int i = 0; i < n; i++) {
        processes[i].completed = 0;
    }
    
    // Calculate waiting time, turnaround time, completion time, and response time
    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].at) {
            idleTime += processes[i].at - currentTime;  // Calculate idle time
            currentTime = processes[i].at;  // Jump to the arrival time of the current process
        }

        displayReadyQueue(processes, n, currentTime);  // Display the ready queue
        
        processes[i].rt = currentTime - processes[i].at;
        processes[i].wt = processes[i].rt;
        processes[i].tat = processes[i].wt + processes[i].bt;
        processes[i].ct = currentTime + processes[i].bt;
        processes[i].completed = 1;  // Mark the process as completed
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

    // Display the Gantt chart
    displayGanttChart(processes, n);
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

    printf("\n\n");
    // Call the FCFS scheduling function
    fcfs(processes, n);

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
Enter Arrival Time for Process 0: 0
Enter Burst Time for Process 0: 2
Enter Process ID for Process 1: 2
Enter Arrival Time for Process 1: 1
Enter Burst Time for Process 1: 2
Enter Process ID for Process 2: 3
Enter Arrival Time for Process 2: 5
Enter Burst Time for Process 2: 3
Enter Process ID for Process 3: 4
Enter Arrival Time for Process 3: 6
Enter Burst Time for Process 3: 4


Ready Queue at time 0: P1 
Ready Queue at time 2: P2 
Ready Queue at time 5: P3 
Ready Queue at time 8: P4 

PROCESS ID      BURST TIME      ARRIVAL TIME    COMPLETION TIME WAITING TIME    TURNAROUND TIME RESPONSE TIME
1               2               0               2               0               2               0
2               2               1               4               1               3               1
3               3               5               8               0               3               0
4               4               6               12              2               6               2

Average Waiting Time: 0.75
Average Turnaround Time: 3.50
Total Idle Time: 1

Gantt Chart:
 0---- 2---- 4---- 5---- 8----12
 | P1  | P2  |Idle | P3  | P4  |
 0---- 2---- 4---- 5---- 8----12

*/