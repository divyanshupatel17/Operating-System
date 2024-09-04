#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_PROCESSES 100

// Structure to represent a process
typedef struct {
    int id;             // Process ID (PID)
    int burst_time;     // Total CPU burst time required
    int arrival_time;   // Time at which the process arrives in the ready queue
    int priority;       // Priority of the process (lower value means higher priority)
    int remaining_time; // Remaining burst time
    int completion_time;// Time at which process completes its execution
    int turnaround_time;// Time difference between completion time and arrival time
    int waiting_time;   // Time difference between turnaround time and burst time
    int response_time;  // Time difference between first execution and arrival time
    int first_run;      // Flag to mark if process has started execution (0: not started, 1: started)
} Process;

// Structure to represent a time slice in the Gantt chart
typedef struct {
    int process_id;     // ID of the process executed in this time slice (-1 for idle)
    int start_time;     // Start time of this slice
    int end_time;       // End time of this slice
} GanttSlice;

// Function to display the ready queue at the current time
void display_ready_queue(Process processes[], int n, int current_time) {
    printf("Ready Queue at time %d: ", current_time);
    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
            printf("P%d ", processes[i].id);
        }
    }
    printf("\n");
}

// Function to find the process with highest priority at a given time
int find_highest_priority_process(Process processes[], int n, int current_time) {
    int highest_priority = INT_MAX;
    int selected_process = -1;

    for (int i = 0; i < n; i++) {
        // Check if the process has arrived and still has remaining execution time
        if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
            // Select this process if it has higher priority (lower priority number)
            // or if it has the same priority but arrived earlier
            if (processes[i].priority < highest_priority ||
                (processes[i].priority == highest_priority && 
                 processes[i].arrival_time < processes[selected_process].arrival_time)) {
                highest_priority = processes[i].priority;
                selected_process = i;
            }
        }
    }

    return selected_process;
}

// Function to implement Priority Preemptive Scheduling
void priority_preemptive_scheduling(Process processes[], int n) {
    int current_time = 0;
    int completed = 0;
    int prev_process = -1;
    GanttSlice gantt[MAX_PROCESSES * 2]; // Gantt chart may have more slices than processes
    int gantt_size = 0;

    while (completed < n) {
        // Display the ready queue at the current time
        display_ready_queue(processes, n, current_time);

        // Find the highest priority process at the current time
        int highest_priority_process = find_highest_priority_process(processes, n, current_time);

        if (highest_priority_process == -1) {
            // No process available, CPU is idle
            if (gantt_size == 0 || gantt[gantt_size - 1].process_id != -1) {
                // Start a new idle slice
                gantt[gantt_size].process_id = -1;
                gantt[gantt_size].start_time = current_time;
            }
            current_time++;
            if (gantt_size > 0 && gantt[gantt_size - 1].process_id == -1) {
                // Extend the current idle slice
                gantt[gantt_size - 1].end_time = current_time;
            } else {
                // Finalize the last idle slice
                gantt[gantt_size - 1].end_time = current_time;
                gantt_size++;
            }
        } else {
            // A process is selected for execution
            Process *p = &processes[highest_priority_process];

            // If this is the first time the process is running, set response time
            if (p->first_run == 0) {
                p->response_time = current_time - p->arrival_time;
                p->first_run = 1;
            }

            // Update Gantt chart
            if (prev_process != highest_priority_process) {
                if (gantt_size > 0 && gantt[gantt_size - 1].process_id == prev_process) {
                    // Extend the last process slice
                    gantt[gantt_size - 1].end_time = current_time;
                }
                gantt[gantt_size].process_id = p->id;
                gantt[gantt_size].start_time = current_time;
                gantt[gantt_size].end_time = current_time + 1;
                gantt_size++;
            } else {
                // Extend the current slice
                gantt[gantt_size - 1].end_time = current_time + 1;
            }

            // Execute the process for one time unit
            p->remaining_time--;
            current_time++;

            // Check if the process has completed
            if (p->remaining_time == 0) {
                completed++;
                p->completion_time = current_time;
                p->turnaround_time = p->completion_time - p->arrival_time;
                p->waiting_time = p->turnaround_time - p->burst_time;
            }

            prev_process = highest_priority_process;
        }
    }

    // Calculate and print average times
    float avg_turnaround_time = 0, avg_waiting_time = 0, avg_response_time = 0;
    for (int i = 0; i < n; i++) {
        avg_turnaround_time += processes[i].turnaround_time;
        avg_waiting_time += processes[i].waiting_time;
        avg_response_time += processes[i].response_time;
    }

    // Print process details
    printf("\nProcess\t AT\t BT\t Priority   CT\t TAT\t WT\t RT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t %d\t %d\t %d\t   %d\t %d\t %d\t %d\n", 
               processes[i].id, processes[i].arrival_time, processes[i].burst_time, 
               processes[i].priority, processes[i].completion_time, 
               processes[i].turnaround_time, processes[i].waiting_time, 
               processes[i].response_time);
    }

    // Print average times
    printf("\nAverage Turnaround Time: %.2f", avg_turnaround_time / n);
    printf("\nAverage Waiting Time: %.2f", avg_waiting_time / n);
    printf("\nAverage Response Time: %.2f\n", avg_response_time / n);

    // Print Gantt Chart
    printf("\nGantt Chart:\n");
    for (int i = 0; i < gantt_size; i++) {
        printf("| ");
        if (gantt[i].process_id == -1) {
            printf("Idle ");
        } else {
            printf("P%d  ", gantt[i].process_id);
        }
    }
    printf("|\n");
    for (int i = 0; i < gantt_size; i++) {
        printf("%d    ", gantt[i].start_time);
    }
    printf("%d\n", gantt[gantt_size - 1].end_time);
}

int main() {
    int n;
    Process processes[MAX_PROCESSES];

    // Input the number of processes
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    // Validate the number of processes
    if (n <= 0 || n > MAX_PROCESSES) {
        printf("Invalid number of processes. Please enter a number between 1 and %d.\n", MAX_PROCESSES);
        return 1;
    }

    // Input details for each process
    for (int i = 0; i < n; i++) {
        printf("Enter details for process %d:\n", i + 1);
        
        // Input Process ID
        printf("Process ID: ");
        scanf("%d", &processes[i].id);
        
        // Input Arrival Time
        printf("Arrival Time: ");
        scanf("%d", &processes[i].arrival_time);
        
        // Input Burst Time
        printf("Burst Time: ");
        scanf("%d", &processes[i].burst_time);
        
        // Input Priority
        printf("Priority (lower value means higher priority): ");
        scanf("%d", &processes[i].priority);
        
        // Initialize remaining time and first_run flag
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].first_run = 0;
    }

    printf("\n\n");
    // Run the priority preemptive scheduling algorithm
    priority_preemptive_scheduling(processes, n);

    return 0;
}

/*
┌──(divyanshu㉿kali)-[~/Desktop]
└─$ gcc temp.c -o code
                                                                                                                                                                                                                                            
┌──(divyanshu㉿kali)-[~/Desktop]
└─$ ./code            
Enter the number of processes: 7
Enter details for process 1:
Process ID: 1
Arrival Time: 0
Burst Time: 8
Priority (lower value means higher priority): 3
Enter details for process 2:
Process ID: 2
Arrival Time: 1
Burst Time: 2
Priority (lower value means higher priority): 4
Enter details for process 3:
Process ID: 3
Arrival Time: 3
Burst Time: 4
Priority (lower value means higher priority): 4
Enter details for process 4:
Process ID: 4
Arrival Time: 4
Burst Time: 1
Priority (lower value means higher priority): 5
Enter details for process 5:
Process ID: 5
Arrival Time: 5
Burst Time: 6
Priority (lower value means higher priority): 2
Enter details for process 6:
Process ID: 6
Arrival Time: 6
Burst Time: 5
Priority (lower value means higher priority): 6
Enter details for process 7:
Process ID: 7
Arrival Time: 10
Burst Time: 1
Priority (lower value means higher priority): 1


Ready Queue at time 0: P1 
Ready Queue at time 1: P1 P2 
Ready Queue at time 2: P1 P2 
Ready Queue at time 3: P1 P2 P3 
Ready Queue at time 4: P1 P2 P3 P4 
Ready Queue at time 5: P1 P2 P3 P4 P5 
Ready Queue at time 6: P1 P2 P3 P4 P5 P6 
Ready Queue at time 7: P1 P2 P3 P4 P5 P6 
Ready Queue at time 8: P1 P2 P3 P4 P5 P6 
Ready Queue at time 9: P1 P2 P3 P4 P5 P6 
Ready Queue at time 10: P1 P2 P3 P4 P5 P6 P7 
Ready Queue at time 11: P1 P2 P3 P4 P5 P6 
Ready Queue at time 12: P1 P2 P3 P4 P6 
Ready Queue at time 13: P1 P2 P3 P4 P6 
Ready Queue at time 14: P1 P2 P3 P4 P6 
Ready Queue at time 15: P2 P3 P4 P6 
Ready Queue at time 16: P2 P3 P4 P6 
Ready Queue at time 17: P3 P4 P6 
Ready Queue at time 18: P3 P4 P6 
Ready Queue at time 19: P3 P4 P6 
Ready Queue at time 20: P3 P4 P6 
Ready Queue at time 21: P4 P6 
Ready Queue at time 22: P6 
Ready Queue at time 23: P6 
Ready Queue at time 24: P6 
Ready Queue at time 25: P6 
Ready Queue at time 26: P6 

Process  AT      BT      Priority   CT   TAT     WT      RT
P1       0       8       3         15    15      7       0
P2       1       2       4         17    16      14      14
P3       3       4       4         21    18      14      14
P4       4       1       5         22    18      17      17
P5       5       6       2         12    7       1       0
P6       6       5       6         27    21      16      16
P7       10      1       1         11    1       0       0

Average Turnaround Time: 13.71
Average Waiting Time: 9.86
Average Response Time: 8.71

Gantt Chart:
| P1  | P5  | P7  | P5  | P1  | P2  | P3  | P4  | P6  |
0    5    10    11    12    15    17    21    22    27

 */