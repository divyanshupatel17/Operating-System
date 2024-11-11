#include <stdio.h>
#include <pthread.h>

// Function to calculate and print the Fibonacci series up to n terms
void* fibonacci_thread(void* arg) {
    int n = *(int*)arg;
    int first = 0, second = 1, next;

    printf("Fibonacci series (using thread):\n");
    for (int i = 0; i < n; i++) {
        if (i <= 1) {
            next = i;
        } else {
            next = first + second;
            first = second;
            second = next;
        }
        printf("%d ", next);
    }
    printf("\n");

    return NULL;
}

int main() {
    pthread_t thread;
    int n;

    printf("Enter the number of terms for the Fibonacci series: ");
    scanf("%d", &n);

    // Create thread to execute the Fibonacci function
    pthread_create(&thread, NULL, fibonacci_thread, &n);

    // Wait for the thread to finish
    pthread_join(thread, NULL);

    return 0;
}


#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void fibonacci_fork(int n) {
    int first = 0, second = 1, next;

    printf("Fibonacci series (using fork):\n");
    for (int i = 0; i < n; i++) {
        if (i <= 1) {
            next = i;
        } else {
            next = first + second;
            first = second;
            second = next;
        }
        printf("%d ", next);
    }
    printf("\n");
}

int main() {
    pid_t pid;
    int n;

    printf("Enter the number of terms for the Fibonacci series: ");
    scanf("%d", &n);

    pid = fork();

    if (pid > 0) {
        // Parent process waits for the child process to complete
        wait(NULL);
    } else if (pid == 0) {
        // Child process calculates and prints the Fibonacci series
        fibonacci_fork(n);
    } else {
        printf("Fork failed!\n");
        return 1;
    }

    return 0;
}
