#include<stdio.h>
#include<limits.h>
#include<stdbool.h>
#include<stdlib.h>

struct process_struct {
    int pid;
    int at;
    int bt;
    int ct, wt, tat, rt, start_time;
    int bt_remaining;
} ps[100];

int gantt_chart[200];  // To store the order of process execution in the Gantt chart
int gantt_time[200];   // To store the corresponding time points for each process in Gantt chart
int gantt_chart_index = 0;

int findmax(int a, int b) {
    return a > b ? a : b;
}

int comparatorAT(const void *a, const void *b) {
    int x = ((struct process_struct *)a)->at;
    int y = ((struct process_struct *)b)->at;
    if (x < y)
        return -1;
    else if (x >= y)
        return 1;
}

int comparatorPID(const void *a, const void *b) {
    int x = ((struct process_struct *)a)->pid;
    int y = ((struct process_struct *)b)->pid;
    if (x < y)
        return -1;
    else if (x >= y)
        return 1;
}

void display_ready_queue(int queue[], int front, int rear) {
    printf("Ready Queue: ");
    for (int i = front; i <= rear; i++) {
        printf("P%d ", queue[i]);
    }
    printf("\n");
}

void display_gantt_chart() {
    printf("\nGantt Chart:\n|");
    for (int i = 0; i < gantt_chart_index; i++) {
        printf(" P%d |", gantt_chart[i]);
    }
    printf("\n");
    //printf("\nTimeline:\n");
    for (int i = 0; i <= gantt_chart_index; i++) {
        printf("%2d  ", gantt_time[i]);
    }
    printf("\n");
}

int main() {
    int n, index;
    bool visited[100] = { false }, is_first_process = true;
    int current_time = 0, completed = 0, tq, total_idle_time = 0;
    printf("Enter total number of processes: ");
    scanf("%d", &n);
    int queue[100], front = -1, rear = -1;
    float sum_tat = 0, sum_wt = 0, sum_rt = 0;

    for (int i = 0; i < n; i++) {
        printf("\nEnter Process %d PID: ", i + 1);
        scanf("%d", &ps[i].pid);

        printf("Enter Process %d Arrival Time: ", i + 1);
        scanf("%d", &ps[i].at);

        printf("Enter Process %d Burst Time: ", i + 1);
        scanf("%d", &ps[i].bt);

        ps[i].bt_remaining = ps[i].bt;
    }

    printf("\nEnter time quanta: ");
    scanf("%d", &tq);

    // Sort structure on the basis of Arrival time in increasing order
    qsort((void *)ps, n, sizeof(struct process_struct), comparatorAT);
    front = rear = 0;
    queue[rear] = 0;
    visited[0] = true;

    while (completed != n) {
        display_ready_queue(queue, front, rear);  // Display ready queue

        index = queue[front];
        front++;

        // Add process to Gantt chart
        if (gantt_chart_index == 0 || gantt_chart[gantt_chart_index - 1] != ps[index].pid) {
            gantt_chart[gantt_chart_index] = ps[index].pid;
            gantt_time[gantt_chart_index] = current_time;
            gantt_chart_index++;
        }

        if (ps[index].bt_remaining == ps[index].bt) {
            ps[index].start_time = findmax(current_time, ps[index].at);
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

        // Check which new processes need to be pushed to Ready Queue from the input list
        for (int i = 1; i < n; i++) {
            if (ps[i].bt_remaining > 0 && ps[i].at <= current_time && visited[i] == false) {
                queue[++rear] = i;
                visited[i] = true;
            }
        }

        // Check if the current process needs to be pushed back to the Ready Queue
        if (ps[index].bt_remaining > 0) {
            queue[++rear] = index;
        }

        // If the queue is empty, just add one process from the list, whose remaining burst time > 0
        if (front > rear) {
            for (int i = 1; i < n; i++) {
                if (ps[i].bt_remaining > 0) {
                    queue[rear++] = i;
                    visited[i] = true;
                    break;
                }
            }
        }
    }

    // Store the final time in the Gantt chart
    gantt_time[gantt_chart_index] = current_time;

    // Sort so that process ID in output comes in Original order (just for interactivity - Not needed otherwise)
    qsort((void *)ps, n, sizeof(struct process_struct), comparatorPID);

    // Output
    printf("\nProcess No.\tPID\tAT\tCPU Burst Time\tStart Time\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++)
        printf("%d\t\t%d\t%d\t%d\t\t%d\t\t%d\t%d\t%d\t%d\n", i + 1, ps[i].pid, ps[i].at, ps[i].bt, ps[i].start_time, ps[i].ct, ps[i].tat, ps[i].wt, ps[i].rt);
    printf("\n");

    printf("\nAverage Turn Around time= %.2f", (float)sum_tat / n);
    printf("\nAverage Waiting Time= %.2f", (float)sum_wt / n);
    printf("\nAverage Response Time= %.2f", (float)sum_rt / n);

    display_gantt_chart();  // Display Gantt chart

    return 0;
}


/*
┌──(divyanshu㉿kali)-[~/Desktop]
└─$ gcc temp.c -o code
                                                                                                                                                                                                                                           
┌──(divyanshu㉿kali)-[~/Desktop]
└─$ ./code            
Enter total number of processes: 5

Enter Process 1 PID: 1
Enter Process 1 Arrival Time: 0
Enter Process 1 Burst Time: 5

Enter Process 2 PID: 2
Enter Process 2 Arrival Time: 1
Enter Process 2 Burst Time: 3

Enter Process 3 PID: 3
Enter Process 3 Arrival Time: 2
Enter Process 3 Burst Time: 1

Enter Process 4 PID: 4
Enter Process 4 Arrival Time: 3
Enter Process 4 Burst Time: 2

Enter Process 5 PID: 5
Enter Process 5 Arrival Time: 4
Enter Process 5 Burst Time: 3

Enter time quanta: 2
Ready Queue: P0 
Ready Queue: P1 P2 P0 
Ready Queue: P2 P0 P3 P4 P1 
Ready Queue: P0 P3 P4 P1 
Ready Queue: P3 P4 P1 P0 
Ready Queue: P4 P1 P0 
Ready Queue: P1 P0 P4 
Ready Queue: P0 P4 
Ready Queue: P4 

Process No.     PID     AT      CPU Burst Time  Start Time      CT      TAT     WT      RT
1               1       0       5               0               13      13      8       0
2               2       1       3               2               12      11      8       1
3               3       2       1               4               5       3       2       2
4               4       3       2               7               9       6       4       4
5               5       4       3               9               14      10      7       5


Average Turn Around time= 8.60
Average Waiting Time= 5.80
Average Response Time= 2.40
Gantt Chart:
| P1 | P2 | P3 | P1 | P4 | P5 | P2 | P1 | P5 |
 0   2   4   5   7   9  11  12  13  14  
*/