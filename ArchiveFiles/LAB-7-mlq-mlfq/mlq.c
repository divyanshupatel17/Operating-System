A technician at a mobile service shop divides all the customers into 4 groups and asks them to wait in separate queues (Q1, Q2, Q3, and Q4) based on their
 mobile model need to be serviced. Assume that there are 6 customers (C1, C2, C3, C4, C5, and C6) with arrival time units (0, 0, 2, 3, 4, 1) and repair
  time required (6, 5, 3, 4, 6, 3) units. Assume the queue number of the customers as (Q1, Q1, Q2, Q3, Q4, Q4). Customers from Q1 require frequent, short 
  check-ups and are attended to every 2 minutes. Customers from Q2 are attended to every 3 minutes and customers from Q3 are attended to every 4 minutes. 
  Customers from Q4 are attended to on a First-Come, First-Serve basis, based on the availability of the technician. Apply a suitable scheduling technique 
  to determine the average turnaround time and average waiting time for the customers based on their repair needs.

#include <stdio.h>
#include <stdbool.h>

#define MAX 6

typedef struct {
    int id;       // Customer ID
    int at;       // Arrival Time
    int rt;       // Repair Time (Burst Time)
    int remaining_time; // Remaining Repair Time
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
                    queue[i].wt = queue[i].tat - queue[i].rt;
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
        queue[i].wt = queue[i].tat - queue[i].rt;
        *current_time += queue[i].remaining_time;
        queue[i].remaining_time = 0;
    }
}

void displayResults(Customer queue[], int n) {
    printf("\nCustomer\tArrival Time\tRepair Time\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for (int i = 0; i < n; i++) {
        printf("C%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               queue[i].id, queue[i].at, queue[i].rt, queue[i].ct, queue[i].tat, queue[i].wt);
    }
}

void calculateOverallAverageTimes(Customer q1[], int n1, Customer q2[], int n2, 
                                  Customer q3[], int n3, Customer q4[], int n4) {
    int total_tat = 0, total_wt = 0;
    int total_customers = n1 + n2 + n3 + n4;

    for (int i = 0; i < n1; i++) {
        total_tat += q1[i].tat;
        total_wt += q1[i].wt;
    }
    for (int i = 0; i < n2; i++) {
        total_tat += q2[i].tat;
        total_wt += q2[i].wt;
    }
    for (int i = 0; i < n3; i++) {
        total_tat += q3[i].tat;
        total_wt += q3[i].wt;
    }
    for (int i = 0; i < n4; i++) {
        total_tat += q4[i].tat;
        total_wt += q4[i].wt;
    }

    float avg_tat = (float)total_tat / total_customers;
    float avg_wt = (float)total_wt / total_customers;

    printf("\nOverall averages for all processes:\n");
    printf("Average TAT = %.2f\n", avg_tat);
    printf("Average WT = %.2f\n", avg_wt);
}

int main() {
    Customer q1[] = { {1, 0, 6, 6}, {2, 0, 5, 5} };  // Q1 (RR, 2 minutes)
    Customer q2[] = { {3, 2, 3, 3} };                // Q2 (RR, 3 minutes)
    Customer q3[] = { {4, 3, 4, 4} };                // Q3 (RR, 4 minutes)
    Customer q4[] = { {5, 4, 6, 6}, {6, 1, 3, 3} };  // Q4 (FCFS)
    
    int current_time = 0;

    // Initialize remaining times for all customers
    for (int i = 0; i < 6; i++) {
        if (i < 2) q1[i].remaining_time = q1[i].rt;
        else if (i < 3) q2[i - 2].remaining_time = q2[i - 2].rt;
        else if (i < 4) q3[i - 3].remaining_time = q3[i - 3].rt;
        else q4[i - 4].remaining_time = q4[i - 4].rt;
    }

    // Processing Q1 (RR with 2 minutes time quantum)
    roundRobin(q1, 2, 2, &current_time);

    // Processing Q2 (RR with 3 minutes time quantum)
    roundRobin(q2, 1, 3, &current_time);

    // Processing Q3 (RR with 4 minutes time quantum)
    roundRobin(q3, 1, 4, &current_time);

    // Processing Q4 (FCFS)
    fcfs(q4, 2, &current_time);

    // Display results for Q1, Q2, Q3, Q4
    printf("\nQueue Q1 (Round Robin, 2 minutes time quantum):");
    displayResults(q1, 2);

    printf("\nQueue Q2 (Round Robin, 3 minutes time quantum):");
    displayResults(q2, 1);

    printf("\nQueue Q3 (Round Robin, 4 minutes time quantum):");
    displayResults(q3, 1);

    printf("\nQueue Q4 (First-Come, First-Serve):");
    displayResults(q4, 2);

    // Calculate and display overall average times
    calculateOverallAverageTimes(q1, 2, q2, 1, q3, 1, q4, 2);

    return 0;
}

/*
// expected output : 

Process No. | Q.No.  |    AT   |   BT   |    CT   |   TAT   |   WT   
1              Q1         0        6         10        10       4     
2              Q1         0        5         11        11       6
3              Q2         2        3         14        12       9   
4              Q3         3        4         18        15       11   
5              Q4         4        6         27        23       17   
6              Q4         1        3         21        20       17

average TAT = 91/6 = 15.17
average WT = 64/6 = 10.67

// output :

┌──(divyanshu㉿kali)-[~/Desktop/OS/7]
└─$ gcc -o mlq mlq.c
                                                                                                                    
┌──(divyanshu㉿kali)-[~/Desktop/OS/7]
└─$ ./mlq           

Queue Q1 (Round Robin, 2 minutes time quantum):
Customer        Arrival Time    Repair Time     Completion Time Turnaround Time Waiting Time
C1              0               6               10              10              4
C2              0               5               11              11              6

Queue Q2 (Round Robin, 3 minutes time quantum):
Customer        Arrival Time    Repair Time     Completion Time Turnaround Time Waiting Time
C3              2               3               14              12              9

Queue Q3 (Round Robin, 4 minutes time quantum):
Customer        Arrival Time    Repair Time     Completion Time Turnaround Time Waiting Time
C4              3               4               18              15              11

Queue Q4 (First-Come, First-Serve):
Customer        Arrival Time    Repair Time     Completion Time Turnaround Time Waiting Time
C6              1               3               21              20              17
C5              4               6               27              23              17

Overall averages for all processes:
Average TAT = 15.17
Average WT = 10.67
                    
*/