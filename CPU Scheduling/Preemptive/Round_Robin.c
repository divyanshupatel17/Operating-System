#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

// Define maximum number of processes
#define MAX_PROCESSES 100

// Structure to represent a process
struct Process {
    int pid;                 // Process ID
    int at;                  // Arrival Time
    int bt;                  // Burst Time
    int ct;                  // Completion Time
    int wt;                  // Waiting Time
    int tat;                 // Turnaround Time
    int rt;                  // Response Time
    int bt_remaining;        // Remaining Burst Time
    int start_time;          // Start Time
};

// Global process array
struct Process ps[MAX_PROCESSES];

// Function prototypes
void readyQueue(struct Process ps[], int n, int current_time, bool visited[], int queue[], int *front, int *rear);
void ganttChart(struct Process ps[], int n, int tq, int current_time);
void calculateAverageTimes(struct Process ps[], int n);

int main() {
    int n, tq;
    int cpu_utilization;
    int queue[MAX_PROCESSES];
    int front = 0, rear = -1;
    bool visited[MAX_PROCESSES] = {false};
    int current_time = 0;
    int completed = 0;
    int total_idle_time = 0;
    int max_completion_time;
    bool is_first_process = true;

    printf("Enter total number of processes: ");
    scanf("%d", &n);
    float sum_tat = 0, sum_wt = 0, sum_rt = 0;

    for (int i = 0; i < n; i++) {
        printf("Enter Process %d Arrival Time: ", i);
        scanf("%d", &ps[i].at);
        ps[i].pid = i;
    }

    for (int i = 0; i < n; i++) {
        printf("Enter Process %d Burst Time: ", i);
        scanf("%d", &ps[i].bt);
        ps[i].bt_remaining = ps[i].bt;
    }

    printf("Enter time quanta: ");
    scanf("%d", &tq);

    // Sort processes based on Arrival Time using a simple bubble sort
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (ps[i].at > ps[j].at) {
                struct Process temp = ps[i];
                ps[i] = ps[j];
                ps[j] = temp;
            }
        }
    }

    // Add the first process to the ready queue
    queue[++rear] = 0;
    visited[0] = true;

    while (completed != n) {
        int index = queue[front++];
        if (ps[index].bt_remaining == ps[index].bt) {
            ps[index].start_time = (current_time > ps[index].at) ? current_time : ps[index].at;
            total_idle_time += (is_first_process == true) ? 0 : ps[index].start_time - current_time;
            current_time = ps[index].start_time;
            is_first_process = false;
        }

        if (ps[index].bt_remaining - tq > 0) {
            ps[index].bt_remaining -= tq;
            current_time += tq;
        } else {
            current_time += ps[index].bt_remaining;
            ps[index].bt_remaining = 0;
            completed++;

            ps[index].ct = current_time;
            ps[index].tat = ps[index].ct - ps[index].at;
            ps[index].wt = ps[index].tat - ps[index].bt;
            ps[index].rt = ps[index].start_time - ps[index].at;

            sum_tat += ps[index].tat;
            sum_wt += ps[index].wt;
            sum_rt += ps[index].rt;
        }

        readyQueue(ps, n, current_time, visited, queue, &front, &rear);

        if (ps[index].bt_remaining > 0) {
            queue[++rear] = index;
        }

        if (front > rear) {
            for (int i = 1; i < n; i++) {
                if (ps[i].bt_remaining > 0) {
                    queue[++rear] = i;
                    visited[i] = true;
                    break;
                }
            }
        }
    }

    max_completion_time = INT_MIN;
    for (int i = 0; i < n; i++) {
        if (ps[i].ct > max_completion_time) {
            max_completion_time = ps[i].ct;
        }
    }

    int length_cycle = max_completion_time - ps[0].at;
    cpu_utilization = (float)(length_cycle - total_idle_time) / length_cycle;

    // Sort processes back to original order based on PID
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (ps[i].pid > ps[j].pid) {
                struct Process temp = ps[i];
                ps[i] = ps[j];
                ps[j] = temp;
            }
        }
    }

    // Output results
    printf("\nProcess No.\tAT\tCPU Burst Time\tStart Time\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t%d\t\t%d\t\t%d\t%d\t%d\t%d\n",
               i, ps[i].at, ps[i].bt, ps[i].start_time, ps[i].ct,
               ps[i].tat, ps[i].wt, ps[i].rt);
    }

    printf("\nAverage Turnaround Time = %.2f", sum_tat / n);
    printf("\nAverage Waiting Time = %.2f", sum_wt / n);
    printf("\nAverage Response Time = %.2f", sum_rt / n);
    printf("\nThroughput = %.2f", (float)n / length_cycle);
    printf("\nCPU Utilization (Percentage) = %.2f%%", cpu_utilization * 100);

    return 0;
}

// Function to add processes to the ready queue
void readyQueue(struct Process ps[], int n, int current_time, bool visited[], int queue[], int *front, int *rear) {
    for (int i = 1; i < n; i++) {
        if (ps[i].bt_remaining > 0 && ps[i].at <= current_time && !visited[i]) {
            queue[++(*rear)] = i;
            visited[i] = true;
        }
    }
}

// Function to print the Gantt chart
void ganttChart(struct Process ps[], int n, int tq, int current_time) {
    printf("\nGantt Chart:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d: %d to %d\n", ps[i].pid, ps[i].start_time, ps[i].ct);
    }
}

/*
Correct Output is : 
Process  AT      BT      CT      TAT     WT      RT
P1       0       5       13      13       8       0
P2       1       3       12      11       8       1
P3       2       1        5       3       2       2
P4       3       2        9       6       4       4
P5       4       3       14      10      10       5



Gantt Chart:
| P1  | P2  | P3  | P1  | P4  | P5  | P2  | P1  | P5  |
0     2     4     6     7     8    10    12    13    14
*/