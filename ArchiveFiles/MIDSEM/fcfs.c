#include <stdio.h>

struct Process {
    int id; // Process ID
    int at; // Arrival Time
    int bt; // Burst Time
    int ct; // Completion Time
    int tat; // Turnaround Time
    int wt; // Waiting Time
};

void findFCFS(struct Process p[], int n) {
    int t = 0; // current time

    for (int i = 0; i < n; i++) {
        if (t < p[i].at) {
            t = p[i].at;
        }

        p[i].ct = t + p[i].bt;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        t = p[i].ct;
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
        {1, 0, 2},
        {2, 1, 2},
        {3, 5, 3},
        {4, 6, 4}
    };
    int n = sizeof(p) / sizeof(p[0]);

    findFCFS(p, n);
    displayProcesses(p, n);
    calculateAverages(p, n);

    return 0;
}

