//  Sleeping Barber

//  gcc -o code sb_semaphore.c -lpthread -lrt
//  ./code


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_CHAIRS 3 // Number of chairs in the barbershop

sem_t barberReady; // Semaphore for barber availability
sem_t customerReady; // Semaphore for customer readiness
sem_t mutex; // Mutex for critical section
int waitingCustomers = 0; // Number of waiting customers

void* barber(void* arg) {
    while (1) {
        // Wait for a customer to be ready
        sem_wait(&customerReady); // Entry to critical section (wait for customer)
        
        // Critical section (serving the customer)
        sem_wait(&mutex); // Lock access to waitingCustomers
        waitingCustomers--; // A customer is being served
        sem_post(&mutex); // Unlock access to waitingCustomers
        
        printf("Barber is cutting hair.\n");
        sleep(rand() % 3); // Simulate haircut
        sem_post(&barberReady); // Signal that barber is ready for next customer
    }
    return NULL;
}

void* customer(void* num) {
    int id = *(int*)num;
    
    sem_wait(&mutex); // Entry to critical section (lock for waitingCustomers)
    if (waitingCustomers < NUM_CHAIRS) {
        waitingCustomers++; // A customer takes a seat
        printf("Customer %d is waiting.\n", id);
        sem_post(&customerReady); // Signal that a customer is ready
        sem_post(&mutex); // Exit from critical section (unlock)
        
        sem_wait(&barberReady); // Wait for the barber to be ready
        printf("Customer %d is getting a haircut.\n", id);
    } else {
        printf("Customer %d left (no seats available).\n", id);
        sem_post(&mutex); // Exit from critical section (unlock)
    }
    return NULL;
}

int main() {
    pthread_t barberThread;
    pthread_t customers[10]; // Up to 10 customers
    int customerIds[10];

    // Initialize semaphores
    sem_init(&barberReady, 0, 0); // Barber is initially not ready
    sem_init(&customerReady, 0, 0); // No customers are initially ready
    sem_init(&mutex, 0, 1); // Initialize mutex

    // Create barber thread
    pthread_create(&barberThread, NULL, barber, NULL);

    // Create customer threads
    for (int i = 0; i < 10; i++) {
        customerIds[i] = i;
        pthread_create(&customers[i], NULL, customer, (void*)&customerIds[i]);
        sleep(rand() % 2); // Simulate arrival time of customers
    }

    // Wait for the barber to finish (not in this case since he runs indefinitely)
    pthread_join(barberThread, NULL);
    for (int i = 0; i < 10; i++) {
        pthread_join(customers[i], NULL);
    }

    // Clean up semaphores (not reached in this case)
    sem_destroy(&barberReady);
    sem_destroy(&customerReady);
    sem_destroy(&mutex);

    return 0;
}


