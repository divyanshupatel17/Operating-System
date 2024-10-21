#include <stdio.h>
#include <limits.h>

struct Process {
    int id;       // Process ID
    int at;       // Arrival Time
    int bt;       // Burst Time
    int pr;       // Priority
    int ct;       // Completion Time
    int tat;      // Turnaround Time
    int wt;       // Waiting Time
    int rt;       // Remaining Time
    int rt_time;  // Response Time
};

void findPriorityPreemptive(struct Process p[], int n) {
    int ct = 0;    // Current Time
    int cp = 0;    // Completed Processes
    int ps[n];     // Processing Status (0 - Not Completed, 1 - Completed)

    // Initialize the processing status array
    for (int i = 0; i < n; i++) {
        ps[i] = 0;
        p[i].rt = p[i].bt;  // Initialize remaining time
        p[i].rt_time = -1;  // Initialize response time to -1 (indicating not started)
    }

    while (cp < n) {
        int mpi = -1; // Minimum Priority Index
        int mp = INT_MAX; // Minimum Priority

        // Find the process with the minimum priority that has arrived and is not yet completed
        for (int i = 0; i < n; i++) {
            if (p[i].at <= ct && !ps[i] && (mpi == -1 || p[i].pr < p[mpi].pr)) {
                mpi = i;
                mp = p[i].pr;
            }
        }

        // If a process is found, decrement its remaining time and update its completion status
        if (mpi != -1) {
            // Calculate response time when the process starts executing for the first time
            if (p[mpi].rt_time == -1) { // If the response time is not set
                p[mpi].rt_time = ct - p[mpi].at; // Set response time to 0
            }

            p[mpi].rt--;
            ct++;
            if (p[mpi].rt == 0) {
                p[mpi].ct = ct;
                p[mpi].tat = p[mpi].ct - p[mpi].at;
                p[mpi].wt = p[mpi].tat - p[mpi].bt;
                ps[mpi] = 1;
                cp++;
            }
        } else {
            ct++;
        }
    }
}

void displayProcesses(struct Process p[], int n) {
    printf("ID\tAT\tBT\tPR\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].id, p[i].at, p[i].bt, p[i].pr, p[i].ct, p[i].tat, p[i].wt, p[i].rt_time);
    }
}

void calculateAverages(struct Process p[], int n) {
    float ttat = 0, twt = 0, trt = 0;
    for (int i = 0; i < n; i++) {
        ttat += p[i].tat;
        twt += p[i].wt;
        if (p[i].rt_time >= 0) {
            trt += p[i].rt_time; // Only include valid response times
        }
    }
    printf("\nAvg TAT: %.2f, Avg WT: %.2f, Avg RT: %.2f\n", ttat / n, twt / n, trt / n);
}

int main() {
    struct Process p[] = {
        {1, 0, 4, 2},
        {2, 1, 3, 1},
        {3, 3, 1, 3},
        {4, 9, 5, 2},
        {5, 12, 2, 1}
    };
    int n = sizeof(p) / sizeof(p[0]);

    findPriorityPreemptive(p, n);
    displayProcesses(p, n);
    calculateAverages(p, n);

    return 0;
}

