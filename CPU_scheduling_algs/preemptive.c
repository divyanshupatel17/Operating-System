/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

// Process structure with all necessary attributes
struct Process {
    int pid;            // Process ID
    int arrivalTime;    // Arrival Time
    int burstTime;      // Burst Time
    int priority;       // Priority (for Priority Scheduling)
    int remainingTime;  // Remaining Burst Time
    int completionTime; // Completion Time
    int turnTime;       // Turnaround Time
    int waitingTime;    // Waiting Time
    int responseTime;   // Response Time
    int startTime;      // First CPU burst start time
};

// Queue structure for Round Robin
struct Queue {
    int items[100];
    int front;
    int rear;
};

// Queue operations
void initQueue(struct Queue *q) {
    q->front = -1;
    q->rear = -1;
}

void push(struct Queue *q, int value) {
    if(q->front == -1) q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
}

int pop(struct Queue *q) {
    int value = q->items[q->front];
    q->front++;
    if(q->front > q->rear) {
        q->front = q->rear = -1;
    }
    return value;
}

int isEmpty(struct Queue *q) {
    return (q->front == -1);
}

// Function to calculate average times
void calculateAverage(struct Process p[], int n, float *avgTurn, float *avgWaiting, float *avgResponse) {
    *avgTurn = 0;
    *avgWaiting = 0;
    *avgResponse = 0;

    for(int i = 0; i < n; i++) {
        *avgTurn += p[i].turnTime;
        *avgWaiting += p[i].waitingTime;
        *avgResponse += p[i].responseTime;
    }

    *avgTurn /= n;
    *avgWaiting /= n;
    *avgResponse /= n;
}

// Display function
void displayResults(struct Process p[], int n, const char* algorithm) {
    printf("\n%s Scheduling Results:\n", algorithm);
    printf("Process No.\tPID\tAT\tBT\tStart Time\tCT\tTAT\tWT\tRT\n");
    for(int i = 0; i < n; i++) {
        printf("%d\t\t%d\t%d\t%d\t%d\t\t%d\t%d\t%d\t%d\n",
               i+1, p[i].pid, p[i].arrivalTime, p[i].burstTime, p[i].startTime,
               p[i].completionTime, p[i].turnTime, p[i].waitingTime, p[i].responseTime);
    }

    float avgTurn, avgWaiting, avgResponse;
    calculateAverage(p, n, &avgTurn, &avgWaiting, &avgResponse);

    printf("\nAverage Turnaround Time = %.2f\n", avgTurn);
    printf("Average Waiting Time = %.2f\n", avgWaiting);
    printf("Average Response Time = %.2f\n", avgResponse);
}

// SRTF Implementation
void SRTF(struct Process p[], int n) {
    int currentTime = 0;
    int completed = 0;
    int *remainingTime = (int*)malloc(n * sizeof(int));
    int *firstTime = (int*)malloc(n * sizeof(int));

    // Initialize
    for(int i = 0; i < n; i++) {
        remainingTime[i] = p[i].burstTime;
        firstTime[i] = 1;
        p[i].responseTime = -1;
    }

    while(completed != n) {
        int minRem = INT_MAX;
        int shortest = -1;

        // Find process with minimum remaining time
        for(int i = 0; i < n; i++) {
            if(p[i].arrivalTime <= currentTime && remainingTime[i] > 0) {
                if(remainingTime[i] < minRem) {
                    minRem = remainingTime[i];
                    shortest = i;
                }
                if(remainingTime[i] == minRem) {
                    if(p[i].arrivalTime < p[shortest].arrivalTime) {
                        shortest = i;
                    }
                }
            }
        }

        if(shortest != -1) {
            // Record start time and response time
            if(firstTime[shortest]) {
                p[shortest].startTime = currentTime;
                p[shortest].responseTime = currentTime - p[shortest].arrivalTime;
                firstTime[shortest] = 0;
            }

            remainingTime[shortest]--;
            currentTime++;

            if(remainingTime[shortest] == 0) {
                completed++;
                p[shortest].completionTime = currentTime;
                p[shortest].turnTime = p[shortest].completionTime - p[shortest].arrivalTime;
                p[shortest].waitingTime = p[shortest].turnTime - p[shortest].burstTime;
            }
        } else {
            currentTime++;
        }
    }

    free(remainingTime);
    free(firstTime);
}

// Priority Preemptive Implementation
void PriorityPreemptive(struct Process p[], int n) {
    int currentTime = 0;
    int completed = 0;
    int *remainingTime = (int*)malloc(n * sizeof(int));
    int *firstTime = (int*)malloc(n * sizeof(int));

    // Initialize
    for(int i = 0; i < n; i++) {
        remainingTime[i] = p[i].burstTime;
        firstTime[i] = 1;
        p[i].responseTime = -1;
    }

    while(completed != n) {
        int highestPriority = INT_MAX;
        int selected = -1;

        // Find process with highest priority
        for(int i = 0; i < n; i++) {
            if(p[i].arrivalTime <= currentTime && remainingTime[i] > 0) {
                if(p[i].priority < highestPriority) {
                    highestPriority = p[i].priority;
                    selected = i;
                }
                if(p[i].priority == highestPriority) {
                    if(p[i].arrivalTime < p[selected].arrivalTime) {
                        selected = i;
                    }
                }
            }
        }

        if(selected != -1) {
            // Record start time and response time
            if(firstTime[selected]) {
                p[selected].startTime = currentTime;
                p[selected].responseTime = currentTime - p[selected].arrivalTime;
                firstTime[selected] = 0;
            }

            remainingTime[selected]--;
            currentTime++;

            if(remainingTime[selected] == 0) {
                completed++;
                p[selected].completionTime = currentTime;
                p[selected].turnTime = p[selected].completionTime - p[selected].arrivalTime;
                p[selected].waitingTime = p[selected].turnTime - p[selected].burstTime;
            }
        } else {
            currentTime++;
        }
    }

    free(remainingTime);
    free(firstTime);
}

// Round Robin Implementation
void RoundRobin(struct Process p[], int n, int timeQuantum) {
    struct Queue q;
    initQueue(&q);

    int currentTime = 0;
    int completed = 0;
    // int *remainingTime = (int*)malloc(n * sizeof(int));
    // int *mark = (int*)calloc(n, sizeof(int));  // Marks if process has been considered for queue
    int remainingTime[n];
    int mark[n];

    // Initialize
    for(int i = 0; i < n; i++) {
        remainingTime[i] = p[i].burstTime;
        p[i].responseTime = -1;
        mark[i] = 0;
    }

    // Find earliest arrival time and push first available process
    int earliestTime = p[0].arrivalTime;
    int firstProcess = 0;
    for(int i = 1; i < n; i++) {
        if(p[i].arrivalTime < earliestTime) {
            earliestTime = p[i].arrivalTime;
            firstProcess = i;
        }
    }

    currentTime = earliestTime;  // Start from earliest arrival time
    push(&q, firstProcess);
    mark[firstProcess] = 1;

    while(completed != n) {
        if(isEmpty(&q)) {
            // Find next available process with minimum arrival time
            int nextProcess = -1;
            int minArrival = INT_MAX;
            for(int i = 0; i < n; i++) {
                if(remainingTime[i] > 0 && !mark[i] && p[i].arrivalTime < minArrival) {
                    minArrival = p[i].arrivalTime;
                    nextProcess = i;
                }
            }
            
            if(nextProcess != -1) {
                currentTime = p[nextProcess].arrivalTime;  // Jump to next arrival
                push(&q, nextProcess);
                mark[nextProcess] = 1;
            }
            // No `continue;` here so newly arrived processes can be checked
        }

        if (!isEmpty(&q)) {
            int current = pop(&q);

            // Record response time only on first execution
            if(p[current].responseTime == -1) {
                p[current].responseTime = currentTime - p[current].arrivalTime;
                p[current].startTime = currentTime;
            }

            // Process execution
            int executionTime = (remainingTime[current] > timeQuantum) ? timeQuantum : remainingTime[current];
            remainingTime[current] -= executionTime;
            currentTime += executionTime;

            // Check for newly arrived processes during this time quantum
            for(int i = 0; i < n; i++) {
                if(remainingTime[i] > 0 && p[i].arrivalTime <= currentTime && mark[i] == 0) {
                    push(&q, i);
                    mark[i] = 1;
                }
            }

            // Handle process completion or re-queue
            if(remainingTime[current] == 0) {
                completed++;
                p[current].completionTime = currentTime;
                p[current].turnTime = p[current].completionTime - p[current].arrivalTime;
                p[current].waitingTime = p[current].turnTime - p[current].burstTime;
            } else {
                push(&q, current);
            }
        }
    }

}


int main() {
    int n, choice;

    // Prompt user for the number of processes
    printf("Enter number of processes: ");
    scanf("%d", &n);

    // Create an array of processes
    struct Process p[n];

    // Input process details
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1; // Assign PID automatically
        printf("Enter Arrival Time, Burst Time, and Priority for process %d: ", i + 1);
        scanf("%d %d %d", &p[i].arrivalTime, &p[i].burstTime, &p[i].priority);
        p[i].remainingTime = p[i].burstTime; // Initialize remaining time
        p[i].startTime = 0; // Initialize start time
        p[i].completionTime = 0; // Initialize completion time
        p[i].turnTime = 0; // Initialize turnaround time
        p[i].waitingTime = 0; // Initialize waiting time
        p[i].responseTime = -1; // Initialize response time
    }

    // Prompt user to choose a scheduling algorithm
    printf("Choose Scheduling Algorithm:\n");
    printf("1. Shortest Remaining Time First (SRTF)\n");
    printf("2. Priority Preemptive\n");
    printf("3. Round Robin\n");
    printf("Choice: ");
    scanf("%d", &choice);

    // Execute selected scheduling algorithm
    switch (choice) {
        case 1:
            SRTF(p, n);
            displayResults(p, n, "SRTF");
            break;
        case 2:
            PriorityPreemptive(p, n);
            displayResults(p, n, "Priority Preemptive");
            break;
        case 3: {
            int timeQuantum;
            printf("Enter Time Quantum for Round Robin: ");
            scanf("%d", &timeQuantum);
            RoundRobin(p, n, timeQuantum);
            displayResults(p, n, "Round Robin");
            break;
        }
        default:
            printf("Invalid choice!\n");
            return 1;
    }

    //displayGanttChart(p, n);
    return 0;
}


/*
Enter number of processes: 5
Enter Arrival Time, Burst Time, and Priority for process 1: 0 8 1
Enter Arrival Time, Burst Time, and Priority for process 2: 5 2 1
Enter Arrival Time, Burst Time, and Priority for process 3: 1 7 1
Enter Arrival Time, Burst Time, and Priority for process 4: 6 3 1
Enter Arrival Time, Burst Time, and Priority for process 5: 8 5 1
Choose Scheduling Algorithm:
1. Shortest Remaining Time First (SRTF)
2. Priority Preemptive
3. Round Robin
Choice: 3
Enter Time Quantum for Round Robin: 3

Round Robin Scheduling Results:
Process No.     PID     AT      BT      Start Time      CT      TAT     WT      RT
1               1       0       8       0               22      22      14      0
2               2       5       2       9               11      6       4       4
3               3       1       7       3               23      22      15      2
4               4       6       3       11              14      8       5       5
5               5       8       5       17              25      17      12      9

Average Turnaround Time = 15.00
Average Waiting Time = 10.00
Average Response Time = 4.00

*/
