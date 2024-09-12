#include <stdio.h>
#include <pthread.h>

void* print_A(void* arg) {
    for (int i = 0; i < 10000; i++) {
        printf("A");
    }
    return NULL;
}

void* print_numbers(void* arg) {
    for (int i = 1; i <= 100000; i++) {
        printf("%d ", i);
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

















