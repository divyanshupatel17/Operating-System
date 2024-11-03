// DINING PHILOSOPHER

//  gcc -o code dp_mutex.c -lpthread -lrt
//  ./code


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

pthread_mutex_t chopsticks[NUM_PHILOSOPHERS]; // Mutex for each chopstick

void* philosopher(void* num) {
    int id = *(int*)num;
    while (1) {
        printf("Philosopher %d is thinking.\n", id);
        sleep(rand() % 3); // Simulate thinking

        // Entry to critical section (pickup chopsticks)
        pthread_mutex_lock(&chopsticks[id]); // Pick up left chopstick
        pthread_mutex_lock(&chopsticks[(id + 1) % NUM_PHILOSOPHERS]); // Pick up right chopstick

        // Critical section (eating)
        printf("Philosopher %d is eating.\n", id);
        sleep(rand() % 3); // Simulate eating

        // Exit from critical section (put down chopsticks)
        pthread_mutex_unlock(&chopsticks[id]); // Put down left chopstick
        pthread_mutex_unlock(&chopsticks[(id + 1) % NUM_PHILOSOPHERS]); // Put down right chopstick
    }
    return NULL;
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    // Initialize mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&chopsticks[i], NULL); // Initialize each chopstick mutex
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

    // Clean up mutexes (not reached in this case)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&chopsticks[i]);
    }

    return 0;
}

