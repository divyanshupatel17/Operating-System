// DINING PHILOSOPHER

//  gcc -o code dp_semaphore.c -lpthread -lrt
//  ./code



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

sem_t chopsticks[NUM_PHILOSOPHERS]; // Semaphore for each chopstick

void* philosopher(void* num) {
    int id = *(int*)num;
    while (1) {
        printf("Philosopher %d is thinking.\n", id);
        sleep(rand() % 3); // Simulate thinking

        // Entry to critical section (pickup chopsticks)
        sem_wait(&chopsticks[id]); // Pick up left chopstick
        sem_wait(&chopsticks[(id + 1) % NUM_PHILOSOPHERS]); // Pick up right chopstick

        // Critical section (eating)
        printf("Philosopher %d is eating.\n", id);
        sleep(rand() % 3); // Simulate eating

        // Exit from critical section (put down chopsticks)
        sem_post(&chopsticks[id]); // Put down left chopstick
        sem_post(&chopsticks[(id + 1) % NUM_PHILOSOPHERS]); // Put down right chopstick
    }
    return NULL;
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    // Initialize semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&chopsticks[i], 0, 1); // Initialize each chopstick semaphore
        ids[i] = i; // Assign philosopher ID
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_create(&philosophers[i], NULL, philosopher, (void*)&ids[i]);
    }

    // Wait for philosopher threads to finish (not in this case since they run indefinitely)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Clean up semaphores (not reached in this case)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&chopsticks[i]);
    }

    return 0;
}

