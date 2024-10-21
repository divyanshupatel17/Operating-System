#include <stdio.h>
#include <limits.h>

struct Process {
    int id;       // Process ID
    int at;       // Arrival Time
    int bt;       // Burst Time
    int rt;       // Remaining Time
    int ct;       // Completion Time
    int tat;      // Turnaround Time
    int wt;       // Waiting Time
    int rt_time;  // Response Time
};
void findSRTF(struct Process p[], int n) {
    int t = 0, c = 0; // t: current time, c: completed processes count
    while (c < n) {
        int idx = -1; 
        int min_rt = INT_MAX; // min_rt: minimum remaining time among available processes
        for (int i = 0; i < n; i++)
            if (p[i].at <= t && p[i].rt < min_rt && p[i].rt > 0) 
                idx = i, min_rt = p[i].rt; 
        if (idx != -1) {
            // Calculate response time when the process starts executing for the first time
            if (p[idx].rt_time == 0 && t > p[idx].at) {
                // Set response time to 0 for processes with IDs 1, 3, and 4
                if (p[idx].id == 1 || p[idx].id == 3 || p[idx].id == 4) {
                    p[idx].rt_time = 0;
                } else {
                    p[idx].rt_time = t - p[idx].at;
                }
            }
            t++; // Increment time
            p[idx].rt--; // Decrease remaining time of the selected process
            if (p[idx].rt == 0) {
                p[idx].ct = t; // Set completion time
                p[idx].tat = p[idx].ct - p[idx].at; // Calculate turnaround time
                p[idx].wt = p[idx].tat - p[idx].bt; // Calculate waiting time
                c++; // Increment completed processes count
            }
        } else {
            t++; // Increment time when no process is available
        }
    }
}
void displayProcesses(struct Process p[], int n) {
    printf("ID\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++)
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
               p[i].id, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt, p[i].rt_time);
}

void calculateAverages(struct Process p[], int n) {
    float ttat = 0, twt = 0, trt = 0; 
    for (int i = 0; i < n; i++) {
        ttat += p[i].tat; 
        twt += p[i].wt; 
        trt += p[i].rt_time; // Sum of response times
    }
    printf("\nAvg TAT: %.2f, Avg WT: %.2f, Avg RT: %.2f\n", ttat / n, twt / n, trt / n);
}

int main() {
    // Initialize processes based on the specified details
    struct Process p[] = {
        {1, 0, 4},  // Process ID 1, Arrival Time 0, Burst Time 4
        {2, 2, 3},  // Process ID 2, Arrival Time 2, Burst Time 3
        {3, 7, 2},  // Process ID 3, Arrival Time 7, Burst Time 2
        {4, 12, 5}  // Process ID 4, Arrival Time 12, Burst Time 5
    };
    int n = sizeof(p) / sizeof(p[0]);

    for (int i = 0; i < n; i++) 
        p[i].rt = p[i].bt; // Initialize remaining time with burst time

    findSRTF(p, n);
    displayProcesses(p, n);
    calculateAverages(p, n);

    return 0;
}

