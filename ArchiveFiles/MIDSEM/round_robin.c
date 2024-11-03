#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

struct process_struct {
    int pid;           // Process ID
    int at;            // Arrival Time
    int bt;            // Burst Time
    int ct;            // Completion Time
    int tat;           // Turnaround Time
    int wt;            // Waiting Time
    int rt;            // Response Time
};

void display_gantt_chart(struct process_struct p[], int n) {
    printf("Gantt Chart:\n");
    for (int i = 0; i < n; i++) {
        printf("| P%d ", p[i].pid);
    }
    printf("|\n");

    for (int i = 0; i < n; i++) {
        if (i == 0) {
            printf("0  ");
        }
        printf("%d  ", p[i].ct);
    }
    printf("\n");
}

void findRoundRobin(struct process_struct p[], int n, int quantum) {
    int current_time = 0;
    int completed = 0;
    bool *finished = calloc(n, sizeof(bool));

    while (completed < n) {
        bool found = false; // Flag to check if any process is found in this iteration
        for (int i = 0; i < n; i++) {
            if (p[i].at <= current_time && !finished[i]) {
                found = true; // A process is found that can be executed
                if (p[i].bt <= quantum) {
                    current_time += p[i].bt;
                    p[i].ct = current_time;
                    p[i].tat = p[i].ct - p[i].at;
                    p[i].wt = p[i].tat - p[i].bt;
                    p[i].rt = current_time - p[i].at;
                    finished[i] = true;
                    completed++;
                } else {
                    current_time += quantum;
                    p[i].bt -= quantum;
                    if (p[i].rt == -1) {
                        p[i].rt = current_time - p[i].at;
                    }
                }
            }
        }
        if (!found) { // If no process is found, increment the current time
            current_time++;
        }
    }

    free(finished); // Free allocated memory for finished array
}

int main() {
    struct process_struct p[] = {
        {1, 0, 5, 0, 0, 0, -1},
        {2, 1, 3, 0, 0, 0, -1},
        {3, 2, 1, 0, 0, 0, -1},
        {4, 3, 2, 0, 0, 0, -1},
        {5, 4, 3, 0, 0, 0, -1}
    };
    int n = sizeof(p) / sizeof(p[0]);
    int quantum = 2;

    findRoundRobin(p, n, quantum);

    float total_tat = 0, total_wt = 0, total_rt = 0;
    for (int i = 0; i < n; i++) {
        printf("Process %d: AT = %d, BT = %d, CT = %d, TAT = %d, WT = %d, RT = %d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt, p[i].rt);
        total_tat += p[i].tat;
        total_wt += p[i].wt;
        total_rt += p[i].rt;
    }

    printf("\nAverage Turn Around Time = %.2f\n", total_tat / n);
    printf("Average Waiting Time = %.2f\n", total_wt / n);
    printf("Average Response Time = %.2f\n", total_rt / n);

    display_gantt_chart(p, n);

    return 0;
}

