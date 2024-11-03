#include <stdio.h>
#include <stdbool.h>

#define MAX 4

typedef struct {
    int id;       // Process ID
    int at;       // Arrival Time
    int bt;       // Burst Time (Repair Time)
    int remaining_time; // Remaining Burst Time
    int wt;       // Waiting Time
    int tat;      // Turnaround Time
    int ct;       // Completion Time
} Customer;

void roundRobin(Customer queue[], int n, int time_quantum, int *current_time) {
    bool done;
    do {
        done = true;
        for (int i = 0; i < n; i++) {
            if (queue[i].remaining_time > 0) {
                done = false;
                if (queue[i].remaining_time > time_quantum) {
                    *current_time += time_quantum;
                    queue[i].remaining_time -= time_quantum;
                } else {
                    *current_time += queue[i].remaining_time;
                    queue[i].ct = *current_time;
                    queue[i].tat = queue[i].ct - queue[i].at;
                    queue[i].wt = queue[i].tat - queue[i].bt;
                    queue[i].remaining_time = 0;
                }
            }
        }
    } while (!done);
}

void fcfs(Customer queue[], int n, int *current_time) {
    // Sort processes by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (queue[i].at > queue[j].at) {
                Customer temp = queue[i];
                queue[i] = queue[j];
                queue[j] = temp;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (*current_time < queue[i].at) {
            *current_time = queue[i].at; // Wait until the process arrives
        }
        queue[i].ct = *current_time + queue[i].remaining_time;
        queue[i].tat = queue[i].ct - queue[i].at;
        queue[i].wt = queue[i].tat - queue[i].bt;
        *current_time += queue[i].remaining_time;
        queue[i].remaining_time = 0;
    }
}

void displayResults(Customer queue[], int n) {
    printf("\nCustomer\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               queue[i].id, queue[i].at, queue[i].bt, queue[i].ct, queue[i].tat, queue[i].wt);
    }
}

void calculateOverallAverageTimes(Customer q1[], int n1, Customer q2[], int n2) {
    int total_tat = 0, total_wt = 0;
    int total_customers = n1 + n2;

    for (int i = 0; i < n1; i++) {
        total_tat += q1[i].tat;
        total_wt += q1[i].wt;
    }
    for (int i = 0; i < n2; i++) {
        total_tat += q2[i].tat;
        total_wt += q2[i].wt;
    }

    float avg_tat = (float)total_tat / total_customers;
    float avg_wt = (float)total_wt / total_customers;

    printf("\nOverall averages for all processes:\n");
    printf("Average TAT = %.2f\n", avg_tat);
    printf("Average WT = %.2f\n", avg_wt);
}

int main() {
    Customer q1[] = { {1, 0, 4, 4}, {2, 0, 3, 3}, {4, 10, 5, 5} };  // Queue 1 (RR, qt = 2)
    Customer q2[] = {3, 0, 8, 8} ;                               // Queue 2 (FCFS)
    
    int current_time = 0;

    // Processing Q1 (Round Robin with 2 minutes time quantum)
    roundRobin(q1, 4, 2, &current_time);

    // Processing Q2 (FCFS)
    fcfs(q2, 2, &current_time);

    // Display results for Q1 and Q2
    printf("\nQueue Q1 (Round Robin, 2 minutes time quantum):");
    displayResults(q1, 4);

    printf("\nQueue Q2 (First-Come, First-Serve):");
    displayResults(q2, 2);

    // Calculate and display overall average times
    calculateOverallAverageTimes(q1, 4, q2, 2);

    return 0;
}

