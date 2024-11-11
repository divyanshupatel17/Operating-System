#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

// Structure to represent a process
struct Process {
    int id;    // Process ID
    int at;    // Arrival Time
    int bt;    // Burst Time
    int pr;    // Priority (for priority scheduling)
    int ct;    // Completion Time
    int tat;   // Turnaround Time
    int wt;    // Waiting Time
    int rt;    // Response Time
};

// Comparison function to sort processes by arrival time
int compareArrival(const void *a, const void *b) {
    return ((struct Process*)a)->at - ((struct Process*)b)->at;
}

// Comparison function to sort processes by burst time
int compareBurst(const void *a, const void *b) {
    return ((struct Process*)a)->bt - ((struct Process*)b)->bt;
}

// Comparison function to sort processes by priority and then by arrival time
int comparePriority(const void *a, const void *b) {
    struct Process *p1 = (struct Process *)a;
    struct Process *p2 = (struct Process *)b;
    if (p1->pr == p2->pr) {
        return p1->at - p2->at; // Sort by arrival time if priorities are the same
    }
    return p1->pr - p2->pr; // Sort by priority
}

// Function to display Gantt chart with idle times
void displayGanttChart(struct Process p[], int n) {
    printf("\nGantt Chart:\n");
    int t = 0;
    for (int i = 0; i < n; i++) {
        if (t < p[i].at) {
            printf("Idle(%d-%d) ", t, p[i].at);
            t = p[i].at;
        }
        printf("P%d(%d-%d) ", p[i].id, t, t + p[i].bt);
        t += p[i].bt;
    }
    printf("\n");
}

// Function for First-Come-First-Serve (FCFS) Scheduling
void findFCFS(struct Process p[], int n) {
    // Sort processes by arrival time
    qsort(p, n, sizeof(struct Process), compareArrival);
    
    int t = 0; // Current time
    for (int i = 0; i < n; i++) {
        // If current time is less than arrival time, idle until process arrives
        if (t < p[i].at) {
            t = p[i].at;
        }
        p[i].ct = t + p[i].bt; // Completion time
        p[i].tat = p[i].ct - p[i].at; // Turnaround time
        p[i].wt = p[i].tat - p[i].bt; // Waiting time
        p[i].rt = (t == p[i].at) ? 0 : t - p[i].at; // Response time
        t = p[i].ct; // Update current time
    }
}

// Function for Shortest Job First (SJF) Non-Preemptive Scheduling
void findSJF(struct Process p[], int n) {
    // Sort processes by arrival time first
    // qsort(p, n, sizeof(struct Process), compareArrival);

    int t = 0, c = 0; // Current time and completed processes count
    int completed[n]; // Array to track completed processes
    for (int i = 0; i < n; i++) completed[i] = 0; // Initialize completed array
    
    while (c < n) {
        int mi = -1, min_bt = INT_MAX; // Min burst time index and value
        for (int i = 0; i < n; i++) {
            // Check for processes that have arrived and are not yet completed
            if (p[i].at <= t && !completed[i] && p[i].bt < min_bt) {
                mi = i; 
                min_bt = p[i].bt; // Update minimum burst time
            }
        }
        if (mi != -1) { // If we found a process to execute
            p[mi].ct = t + p[mi].bt; // Completion time
            p[mi].tat = p[mi].ct - p[mi].at; // Turnaround time
            p[mi].wt = p[mi].tat - p[mi].bt; // Waiting time
            p[mi].rt = (t == p[mi].at) ? 0 : t - p[mi].at; // Response time
            completed[mi] = 1; // Mark this process as completed
            c++; // Increment completed count
            t = p[mi].ct; // Update current time
        } else {
            t++; // If no process is ready, increment time
        }
    }
}

// Function for Priority Non-Preemptive Scheduling
void findPriority(struct Process p[], int n) {
    // Sort processes by arrival time first
    qsort(p, n, sizeof(struct Process), compareArrival);

    int t = 0, c = 0; // Current time and completed processes count
    int completed[n]; // Array to track completed processes
    for (int i = 0; i < n; i++) completed[i] = 0; // Initialize completed array

    while (c < n) {
        int mi = -1, min_pr = INT_MAX; // Min priority index and value
        for (int i = 0; i < n; i++) {
            // Check for processes that have arrived and are not yet completed
            if (p[i].at <= t && !completed[i] && p[i].pr < min_pr) {
                mi = i; 
                min_pr = p[i].pr; // Update minimum priority
            }
        }
        if (mi != -1) { // If we found a process to execute
            p[mi].ct = t + p[mi].bt; // Completion time
            p[mi].tat = p[mi].ct - p[mi].at; // Turnaround time
            p[mi].wt = p[mi].tat - p[mi].bt; // Waiting time
            p[mi].rt = (t == p[mi].at) ? 0 : t - p[mi].at; // Response time
            completed[mi] = 1; // Mark this process as completed
            c++; // Increment completed count
            t = p[mi].ct; // Update current time
        } else {
            t++; // If no process is ready, increment time
        }
    }
}

// Function to display processes and their calculated times
void displayProcesses(struct Process p[], int n) {
    printf("ID\tAT\tBT\tPR\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].at, p[i].bt, p[i].pr, p[i].ct, p[i].tat, p[i].wt, p[i].rt);
    }
}

// Function to calculate and display averages of TAT, WT, and RT
void calculateAverages(struct Process p[], int n) {
    float total_tat = 0, total_wt = 0, total_rt = 0;
    for (int i = 0; i < n; i++) {
        total_tat += p[i].tat;
        total_wt += p[i].wt;
        total_rt += p[i].rt;
    }
    printf("\nAverage TAT: %.2f, Average WT: %.2f, Average RT: %.2f\n",
           total_tat / n, total_wt / n, total_rt / n);
}

int main() {
    int n, choice;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    struct Process p[n]; // Array of processes
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1; // Assign process ID
        printf("Enter AT, BT, PR for process %d: ", i + 1);
        scanf("%d %d %d", &p[i].at, &p[i].bt, &p[i].pr); // Input arrival time, burst time, and priority
    }
    printf("Choose Scheduling Algorithm (1 for FCFS, 2 for SJF, 3 for Priority): ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: findFCFS(p, n); break;          // Call FCFS scheduling function
        case 2: findSJF(p, n); break;           // Call SJF scheduling function
        case 3: findPriority(p, n); break;      // Call Priority scheduling function
        default: printf("Invalid choice!\n"); return 1; // Handle invalid choice
    }

    displayProcesses(p, n); // Display process information
    calculateAverages(p, n); // Calculate and display averages
    displayGanttChart(p, n); // Display Gantt chart

    return 0; // Exit program
}
