#include <stdio.h>

struct Process {
    int id; // Process ID
    int at; // Arrival Time
    int bt; // Burst Time
    int ct; // Completion Time
    int tat; // Turnaround Time
    int wt; // Waiting Time
};

void findSJF(struct Process p[], int n) {
    int t = 0; // Current time
    int c = 0; // Count of completed processes
    int completed[n];
    for (int i = 0; i < n; i++) {
        completed[i] = 0;
    }
    while (c < n) {
        int minIndex = -1, minBT = __INT_MAX__;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= t && !completed[i] && p[i].bt < minBT) {
                minIndex = i;
                minBT = p[i].bt;
            }
        }
        if (minIndex != -1) {
            p[minIndex].ct = t + p[minIndex].bt;
            p[minIndex].tat = p[minIndex].ct - p[minIndex].at;
            p[minIndex].wt = p[minIndex].tat - p[minIndex].bt;
            completed[minIndex] = 1;
            c++; // Increment completed process count
            t = p[minIndex].ct; // Update current time
        } else {
            t++; // If no process is ready, increment time
        }
    }
}

void displayProcesses(struct Process p[], int n) {
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
}

void calculateAverages(struct Process p[], int n) {
    float tat = 0, wt = 0;
    for (int i = 0; i < n; i++) {
        tat += p[i].tat;
        wt += p[i].wt;
    }
    printf("\nAvg TAT: %.2f, Avg WT: %.2f\n", tat / n, wt / n);
}

int main() {
    struct Process p[] = {
        {1, 1, 3},
        {2, 2, 4},
        {3, 1, 2},
        {4, 4, 4}
    };
    int n = sizeof(p) / sizeof(p[0]);

    findSJF(p, n);
    displayProcesses(p, n);
    calculateAverages(p, n);

    return 0;
}


