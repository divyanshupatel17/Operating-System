#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define NUM_ITERATIONS 5

int flag[2];
int turn;
int i = 14;            // my reg no : 23BAI1214

void lock_init(){
    flag[0] = flag[1] = 0;
    turn = 0;
}
void lock(int self){
    flag[self] = 1;
    turn = 1 - self;
    while (flag[1 - self] == 1 && turn == 1 - self);
}
void unlock(int self){
    flag[self] = 0;
}
void* process_A(void* arg){
    for (int j = 0; j < NUM_ITERATIONS; j++) {
        lock(0);
        i++;
        printf("Process A incremented i to: %d\n", i);
        unlock(0);
    }
    return NULL;
}
void* process_B(void* arg){
    for (int j = 0; j < NUM_ITERATIONS; j++) {
        lock(1);
        i--;
        printf("Process B decremented i to: %d\n", i);
        unlock(1);
    }
    return NULL;
}
int main(){
    pthread_t thread_A, thread_B;
    
    lock_init();

    printf("Initial value of i: %d\n", i);

    pthread_create(&thread_A, NULL, process_A, NULL);
    pthread_create(&thread_B, NULL, process_B, NULL);

    pthread_join(thread_A, NULL);
    pthread_join(thread_B, NULL);

    printf("Final value of i: %d\n", i);

    return 0;
}


