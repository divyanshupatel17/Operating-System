#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

int flag[2] = {0, 0};
int turn = 0;

void lock(int id) {
    flag[id] = 1;
    turn = 1 - id;
    while (flag[1-id] && turn == 1-id);
}

void unlock(int id) {
    flag[id] = 0;
}

void* print_A(void* arg) {
    for (int i = 0; i < 10000; i++) {
        lock(0);
        printf("A");
        unlock(0);
    }
    return NULL;
}

void* print_numbers(void* arg) {
    for (int i = 1; i <= 100000; i++) {
        lock(1);
        printf("%d ", i);
        unlock(1);
    }
    return NULL;
}

int main() {
    pthread_t thread_A, thread_B;

    pthread_create(&thread_A, NULL, print_A, NULL);
    pthread_create(&thread_B, NULL, print_numbers, NULL);

    pthread_join(thread_A, NULL);
    pthread_join(thread_B, NULL);
    printf("\n");
    return 0;
}


