// PRODUCER CONSUMER

//  gcc -o code pc_semaphore.c -lpthread -lrt
//  ./code


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0; // Index for the next produced item
int out = 0; // Index for the next consumed item

sem_t empty; // Semaphore for counting empty slots
sem_t full;  // Semaphore for counting full slots
pthread_mutex_t mutex; // Mutex for mutual exclusion

void* producer(void* param) {
    for (int i = 0; i < 10; i++) {
        sem_wait(&empty); // Wait for empty slot
        pthread_mutex_lock(&mutex); // Lock the buffer

        buffer[in] = i; // Produce an item
        printf("Produced: %d\n", i);
        in = (in + 1) % BUFFER_SIZE; // Move to the next slot

        pthread_mutex_unlock(&mutex); // Unlock the buffer
        sem_post(&full); // Signal that a new item is available
        sleep(rand() % 2); // Simulate variable production time
    }
    return NULL;
}

void* consumer(void* param) {
    for (int i = 0; i < 10; i++) {
        sem_wait(&full); // Wait for a full slot
        pthread_mutex_lock(&mutex); // Lock the buffer

        int item = buffer[out]; // Consume an item
        printf("Consumed: %d\n", item);
        out = (out + 1) % BUFFER_SIZE; // Move to the next slot

        pthread_mutex_unlock(&mutex); // Unlock the buffer
        sem_post(&empty); // Signal that an item has been consumed
        sleep(rand() % 2); // Simulate variable consumption time
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    sem_init(&empty, 0, BUFFER_SIZE); // Initialize empty slots
    sem_init(&full, 0, 0); // Initialize full slots
    pthread_mutex_init(&mutex, NULL); // Initialize mutex

    pthread_create(&prod, NULL, producer, NULL); // Create producer thread
    pthread_create(&cons, NULL, consumer, NULL); // Create consumer thread

    pthread_join(prod, NULL); // Wait for producer to finish
    pthread_join(cons, NULL); // Wait for consumer to finish

    sem_destroy(&empty); // Clean up semaphores
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex); // Clean up mutex

    return 0;
}

