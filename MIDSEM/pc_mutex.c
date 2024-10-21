// PRODUCER CONSUMER

//  gcc -o code pc_mutex.c -lpthread -lrt
//  ./code


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0; // Index for the next produced item
int out = 0; // Index for the next consumed item
int count = 0; // Count of items in the buffer

pthread_mutex_t mutex; // Mutex for mutual exclusion
pthread_cond_t empty; // Condition variable for empty buffer
pthread_cond_t full; // Condition variable for full buffer

void* producer(void* param) {
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex); // Lock the buffer (entry to critical section)

        while (count == BUFFER_SIZE) // Wait if the buffer is full
            pthread_cond_wait(&empty, &mutex);

        buffer[in] = i; // Produce an item
        printf("Produced: %d\n", i);
        in = (in + 1) % BUFFER_SIZE; // Move to the next slot
        count++; // Increment count

        pthread_cond_signal(&full); // Signal that a new item is available
        pthread_mutex_unlock(&mutex); // Unlock the buffer (exit from critical section)
        sleep(rand() % 2); // Simulate variable production time
    }
    return NULL;
}

void* consumer(void* param) {
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex); // Lock the buffer (entry to critical section)

        while (count == 0) // Wait if the buffer is empty
            pthread_cond_wait(&full, &mutex);

        int item = buffer[out]; // Consume an item
        printf("Consumed: %d\n", item);
        out = (out + 1) % BUFFER_SIZE; // Move to the next slot
        count--; // Decrement count

        pthread_cond_signal(&empty); // Signal that an item has been consumed
        pthread_mutex_unlock(&mutex); // Unlock the buffer (exit from critical section)
        sleep(rand() % 2); // Simulate variable consumption time
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    pthread_mutex_init(&mutex, NULL); // Initialize mutex
    pthread_cond_init(&empty, NULL); // Initialize condition variable for empty
    pthread_cond_init(&full, NULL); // Initialize condition variable for full

    pthread_create(&prod, NULL, producer, NULL); // Create producer thread
    pthread_create(&cons, NULL, consumer, NULL); // Create consumer thre

    pthread_join(prod, NULL); // Wait for producer to finish
    pthread_join(cons, NULL); // Wait for consumer to finish

    pthread_mutex_destroy(&mutex); // Clean up mutex
    pthread_cond_destroy(&empty); // Clean up condition variable for empty
    pthread_cond_destroy(&full); // Clean up condition variable for full

    return 0;
}

