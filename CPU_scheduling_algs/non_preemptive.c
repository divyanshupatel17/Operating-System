#include <stdio.h>
#include <limits.h>

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
    int t = 0;
    for (int i = 0; i < n; i++) {
        if (t < p[i].at) t = p[i].at;
        p[i].ct = t + p[i].bt;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        p[i].rt = p[i].wt;
        t = p[i].ct;
    }
}

// Function for Shortest Job First (SJF) Non-Preemptive Scheduling
void findSJF(struct Process p[], int n) {
    int t = 0, c = 0;
    int completed[n];
    for (int i = 0; i < n; i++) completed[i] = 0;
    
    while (c < n) {
        int mi = -1, min_bt = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= t && !completed[i] && p[i].bt < min_bt) {
                mi = i; min_bt = p[i].bt;
            }
        }
        if (mi != -1) {
            p[mi].ct = t + p[mi].bt;
            p[mi].tat = p[mi].ct - p[mi].at;
            p[mi].wt = p[mi].tat - p[mi].bt;
            p[mi].rt = p[mi].wt;
            completed[mi] = 1;
            c++; t = p[mi].ct;
        } else t++;
    }
}

// Function for Priority Non-Preemptive Scheduling
void findPriority(struct Process p[], int n) {
    int t = 0, c = 0;
    int completed[n];
    for (int i = 0; i < n; i++) completed[i] = 0;

    while (c < n) {
        int mi = -1, min_pr = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= t && !completed[i] && p[i].pr < min_pr) {
                mi = i; min_pr = p[i].pr;
            }
        }
        if (mi != -1) {
            p[mi].ct = t + p[mi].bt;
            p[mi].tat = p[mi].ct - p[mi].at;
            p[mi].wt = p[mi].tat - p[mi].bt;
            p[mi].rt = p[mi].wt;
            completed[mi] = 1;
            c++; t = p[mi].ct;
        } else t++;
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
    struct Process p[n];
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("Enter AT, BT, PR for process %d: ", i + 1);
        scanf("%d %d %d", &p[i].at, &p[i].bt, &p[i].pr);
    }
    printf("Choose Scheduling Algorithm (1 for FCFS, 2 for SJF, 3 for Priority): ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: findFCFS(p, n); break;
        case 2: findSJF(p, n); break;
        case 3: findPriority(p, n); break;
        default: printf("Invalid choice!\n"); return 1;
    }

    displayProcesses(p, n);
    calculateAverages(p, n);
    displayGanttChart(p, n);

    return 0;
}

