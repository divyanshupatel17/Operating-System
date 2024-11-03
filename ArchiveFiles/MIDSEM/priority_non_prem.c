#include <stdio.h>
#include <limits.h>

struct Process {
    int id, pr, at, bt, ct, tat, wt;
};

void findPriorityScheduling(struct Process p[], int n) {
    int t = 0; // Current time
    int c = 0; // Count of completed processes
    int ps[n]; 
    for(int i = 0; i < n; i++) ps[i] = 0;

    while (c < n) {
        int mi = -1; // Index of the selected process
        int mp = INT_MAX; // Minimum priority
        for (int i = 0; i < n; i++)
            if (p[i].at <= t && !ps[i] && p[i].pr < mp) 
                mi = i, mp = p[i].pr; 
        if (mi != -1) {
            p[mi].ct = t + p[mi].bt; 
            p[mi].tat = p[mi].ct - p[mi].at; 
            p[mi].wt = p[mi].tat - p[mi].bt; 
            ps[mi] = 1; 
            c++; 
            t = p[mi].ct; 
        } else {
            t++; 
        }
    }
}

void displayProcesses(struct Process p[], int n) {
    printf("ID\tAT\tBT\tPR\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++)
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", p[i].id, p[i].at, p[i].bt, p[i].pr, p[i].ct, p[i].tat, p[i].wt);
}

void calculateAverages(struct Process p[], int n) {
    float ttat = 0, twt = 0; 
    for (int i = 0; i < n; i++) {
        ttat += p[i].tat; 
        twt += p[i].wt; 
    }
    printf("\nAvg TAT: %.2f, Avg WT: %.2f\n", ttat / n, twt / n); 
}

int main() {
    struct Process p[] = {{1, 2, 0, 11}, {2, 0, 5, 28}, {3, 3, 12, 2}, {4, 1, 2, 10}, {5, 4, 9, 16}};
    int n = sizeof(p) / sizeof(p[0]);

    findPriorityScheduling(p, n);
    displayProcesses(p, n);
    calculateAverages(p, n);

    return 0;
}

