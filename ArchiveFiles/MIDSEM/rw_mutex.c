//  Readers Writers

//  gcc -o code rw_mutex.c -lpthread -lrt
//  ./code

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define READERS 5
#define WRITERS 2

pthread_mutex_t readCountMutex; // Protects access to readCount
pthread_mutex_t writeLock; // Allows only one writer at a time
int readCount = 0; // Count of active readers

void* reader(void* num) {
    int id = *(int*)num;
    while (1) {
        // Entry to critical section (acquire readCountMutex)
        pthread_mutex_lock(&readCountMutex); // Lock access to readCount
        readCount++; // Increment the number of readers
        if (readCount == 1) {
            pthread_mutex_lock(&writeLock); // First reader locks the writeLock
        }
        pthread_mutex_unlock(&readCountMutex); // Unlock access to readCount

        // Critical section (reading)
        printf("Reader %d is reading.\n", id);
        sleep(rand() % 3); // Simulate reading

        // Exit from critical section (release readCountMutex)
        pthread_mutex_lock(&readCountMutex); // Lock access to readCount
        readCount--; // Decrement the number of readers
        if (readCount == 0) {
            pthread_mutex_unlock(&writeLock); // Last reader releases the writeLock
        }
        pthread_mutex_unlock(&readCountMutex); // Unlock access to readCount
        sleep(rand() % 2); // Simulate idle time
    }
    return NULL;
}

void* writer(void* num) {
    int id = *(int*)num;
    while (1) {
        // Entry to critical section (acquire writeLock)
        pthread_mutex_lock(&writeLock); // Lock access for writing

        // Critical section (writing)
        printf("Writer %d is writing.\n", id);
        sleep(rand() % 3); // Simulate writing

        // Exit from critical section (release writeLock)
        pthread_mutex_unlock(&writeLock); // Unlock access for writing
        sleep(rand() % 2); // Simulate idle time
    }
    return NULL;
}

int main() {
    pthread_t readers[READERS], writers[WRITERS];
    int readerIds[READERS], writerIds[WRITERS];

    // Initialize mutexes
    pthread_mutex_init(&readCountMutex, NULL); // Initialize mutex for readCount
    pthread_mutex_init(&writeLock, NULL); // Initialize writeLock

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

    // Clean up mutexes (not reached in this case)
    pthread_mutex_destroy(&readCountMutex);
    pthread_mutex_destroy(&writeLock);

    return 0;
}

