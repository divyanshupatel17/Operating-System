//  Readers Writers

//  gcc -o code rw_semaphore.c -lpthread -lrt
//  ./code

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define READERS 5
#define WRITERS 2

sem_t mutex; // Protects access to readCount
sem_t writeLock; // Allows only one writer at a time
int readCount = 0; // Count of active readers

void* reader(void* num) {
    int id = *(int*)num;
    while (1) {
        // Entry to critical section (acquire readLock)
        sem_wait(&mutex); // Lock access to readCount
        readCount++; // Increment the number of readers
        if (readCount == 1) {
            sem_wait(&writeLock); // First reader locks the writeLock
        }
        sem_post(&mutex); // Unlock access to readCount

        // Critical section (reading)
        printf("Reader %d is reading.\n", id);
        sleep(rand() % 3); // Simulate reading

        // Exit from critical section (release readLock)
        sem_wait(&mutex); // Lock access to readCount
        readCount--; // Decrement the number of readers
        if (readCount == 0) {
            sem_post(&writeLock); // Last reader releases the writeLock
        }
        sem_post(&mutex); // Unlock access to readCount
        sleep(rand() % 2); // Simulate idle time
    }
    return NULL;
}

void* writer(void* num) {
    int id = *(int*)num;
    while (1) {
        // Entry to critical section (acquire writeLock)
        sem_wait(&writeLock); // Lock access for writing

        // Critical section (writing)
        printf("Writer %d is writing.\n", id);
        sleep(rand() % 3); // Simulate writing

        // Exit from critical section (release writeLock)
        sem_post(&writeLock); // Unlock access for writing
        sleep(rand() % 2); // Simulate idle time
    }
    return NULL;
}

int main() {
    pthread_t readers[READERS], writers[WRITERS];
    int readerIds[READERS], writerIds[WRITERS];

    // Initialize semaphores
    sem_init(&mutex, 0, 1); // Initialize mutex for readCount
    sem_init(&writeLock, 0, 1); // Initialize writeLock

    // Create reader threads
    for (int i = 0; i < READERS; i++) {
        readerIds[i] = i;
        pthread_create(&readers[i], NULL, reader, (void*)&readerIds[i]);
    }

    // Create writer threads
    for (int i = 0; i < WRITERS; i++) {
        writerIds[i] = i;
        pthread_create(&writers[i], NULL, writer, (void*)&writerIds[i]);
    }

    // Wait for threads to finish (not in this case since they run indefinitely)
    for (int i = 0; i < READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    // Clean up semaphores (not reached in this case)
    sem_destroy(&mutex);
    sem_destroy(&writeLock);

    return 0;
}

