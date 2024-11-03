#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

struct process_struct {
    int pid;           // Process ID
    int at;            // Arrival Time
    int bt;            // Burst Time
    int ct;            // Completion Time
    int wt;            // Waiting Time
    int tat;           // Turnaround Time
    int rt;            // Response Time
    int start_time;    // Start Time
    int bt_remaining;  // Remaining Burst Time
} ps[100];

int gantt_chart[200];  // Stores the order of process execution for the Gantt chart
int gantt_time[200];   // Stores the corresponding time points for the Gantt chart
int gantt_chart_index = 0;  // Tracks the index for the Gantt chart

// Function to find the maximum of two integers
int findmax(int a, int b) {
    return a > b ? a : b;
}

// Comparator function for qsort to sort processes by arrival time
int comparatorAT(const void *a, const void *b) {
    int x = ((struct process_struct *)a)->at;
    int y = ((struct process_struct *)b)->at;
    return x - y;
}

// Comparator function for qsort to sort processes by process ID
int comparatorPID(const void *a, const void *b) {
    int x = ((struct process_struct *)a)->pid;
    int y = ((struct process_struct *)b)->pid;
    return x - y;
}

// Function to display the ready queue
void display_ready_queue(int queue[], int front, int rear) {
    printf("Ready Queue: ");
    for (int i = front; i <= rear; i++) {
        printf("P%d ", queue[i]);
    }
    printf("\n");
}

// Function to display the Gantt chart
void display_gantt_chart() {
    printf("\nGantt Chart:\n|");
    for (int i = 0; i < gantt_chart_index; i++) {
        printf(" P%d |", gantt_chart[i]);
    }
    printf("\n");
    for (int i = 0; i <= gantt_chart_index; i++) {
        printf("%2d  ", gantt_time[i]);
    }
    printf("\n");
}

int main() {
    int n, index;
    bool visited[100] = {false}, is_first_process = true;
    int current_time = 0, completed = 0, tq, total_idle_time = 0;
    
    printf("Enter total number of processes: ");
    scanf("%d", &n);
    
    int queue[100], front = -1, rear = -1;
    float sum_tat = 0, sum_wt = 0, sum_rt = 0;

    // Input process data
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

    // Sort processes based on arrival time
    qsort((void *)ps, n, sizeof(struct process_struct), comparatorAT);

    // Initialize the ready queue
    front = rear = 0;
    queue[rear] = 0;
    visited[0] = true;

    while (completed != n) {
        display_ready_queue(queue, front, rear);  // Display the ready queue

        index = queue[front];
        front++;

        // Add process to the Gantt chart if it's not already there
        if (gantt_chart_index == 0 || gantt_chart[gantt_chart_index - 1] != ps[index].pid) {
            gantt_chart[gantt_chart_index] = ps[index].pid;
            gantt_time[gantt_chart_index] = current_time;
            gantt_chart_index++;
        }

        // If it's the first execution of the process
        if (ps[index].bt_remaining == ps[index].bt) {
            ps[index].start_time = findmax(current_time, ps[index].at);
            total_idle_time += (is_first_process ? 0 : ps[index].start_time - current_time);
            current_time = ps[index].start_time;
            is_first_process = false;
        }

        // Execute the process for the time quantum or less
        if (ps[index].bt_remaining > tq) {
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

        // Add new processes to the ready queue if they have arrived
        for (int i = 1; i < n; i++) {
            if (ps[i].bt_remaining > 0 && ps[i].at <= current_time && !visited[i]) {
                queue[++rear] = i;
                visited[i] = true;
            }
        }

        // If the current process is not finished, add it back to the ready queue
        if (ps[index].bt_remaining > 0) {
            queue[++rear] = index;
        }

        // If the queue is empty, add the next process that has not completed
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

    // Sort processes by PID for output
    qsort((void *)ps, n, sizeof(struct process_struct), comparatorPID);

    // Display process details
    printf("\nProcess No.\tPID\tAT\tBT\tStart Time\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t%d\t%d\t%d\t\t%d\t%d\t%d\t%d\n", i + 1, ps[i].pid, ps[i].at, ps[i].bt, ps[i].start_time, ps[i].ct, ps[i].tat, ps[i].wt, ps[i].rt);
    }

    printf("\nAverage Turn Around Time = %.2f", sum_tat / n);
    printf("\nAverage Waiting Time = %.2f", sum_wt / n);
    printf("\nAverage Response Time = %.2f\n", sum_rt / n);

    // Display the Gantt chart
    display_gantt_chart();

    return 0;
}

/*                            
┌──(divyanshu㉿kali)-[~/Desktop]
└─$ gcc temp.c -o code
                                                                                                                    
┌──(divyanshu㉿kali)-[~/Desktop]
└─$ ./code            
Enter total number of processes: 4

Enter Process 1 PID: 1
Enter Process 1 Arrival Time: 0
Enter Process 1 Burst Time: 8

Enter Process 2 PID: 2 
Enter Process 2 Arrival Time: 0
Enter Process 2 Burst Time: 5

Enter Process 3 PID: 3
Enter Process 3 Arrival Time: 0
Enter Process 3 Burst Time: 10

Enter Process 4 PID: 4
Enter Process 4 Arrival Time: 0
Enter Process 4 Burst Time: 11

Enter time quanta: 3
Ready Queue: P0 
Ready Queue: P1 P2 P3 P0 
Ready Queue: P2 P3 P0 P1 
Ready Queue: P3 P0 P1 P2 
Ready Queue: P0 P1 P2 P3 
Ready Queue: P1 P2 P3 P0 
Ready Queue: P2 P3 P0 
Ready Queue: P3 P0 P2 
Ready Queue: P0 P2 P3 
Ready Queue: P2 P3 
Ready Queue: P3 P2 
Ready Queue: P2 P3 
Ready Queue: P3 

Process No.     PID     AT      BT      Start Time      CT      TAT     WT      RT
1               1       0       8       0               25      25      17      0
2               2       0       5       3               17      17      12      3
3               3       0       10      6               32      32      22      6
4               4       0       11      9               34      34      23      9

Average Turn Around Time = 27.00
Average Waiting Time = 18.50
Average Response Time = 4.50

Gantt Chart:
| P1 | P2 | P3 | P4 | P1 | P2 | P3 | P4 | P1 | P3 | P4 | P3 | P4 |
 0   3   6   9  12  15  17  20  23  25  28  31  32  34  
                                                          
*/