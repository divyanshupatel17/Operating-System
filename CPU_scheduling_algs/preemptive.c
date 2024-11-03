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
    int *remainingTime = (int*)malloc(n * sizeof(int));
    int *mark = (int*)calloc(n, sizeof(int));
    
    // Initialize
    for(int i = 0; i < n; i++) {
        remainingTime[i] = p[i].burstTime;
        p[i].responseTime = -1;
    }
    
    // Push first process
    push(&q, 0);
    mark[0] = 1;
    
    while(completed != n) {
        int current = pop(&q);
        
        // Record start time and response time
        if(remainingTime[current] == p[current].burstTime) {
            p[current].startTime = currentTime > p[current].arrivalTime ? 
                                 currentTime : p[current].arrivalTime;
            currentTime = p[current].startTime;
            if(p[current].responseTime == -1) {
                p[current].responseTime = p[current].startTime - p[current].arrivalTime;
            }
        }
        
        // Process execution
        int executionTime = (remainingTime[current] > timeQuantum) ? timeQuantum : remainingTime[current];
        remainingTime[current] -= executionTime;
        currentTime += executionTime;
        
        // Check for newly arrived processes
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
        
        // Handle empty queue
        if(isEmpty(&q) && completed != n) {
            for(int i = 0; i < n; i++) {
                if(remainingTime[i] > 0) {
                    push(&q, i);
                    mark[i] = 1;
                    break;
                }
            }
        }
    }
    
    free(remainingTime);
    free(mark);
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
