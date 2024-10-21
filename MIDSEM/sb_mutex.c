//  Sleeping Barber

//  gcc -o code sb_mutex.c -lpthread -lrt
//  ./code

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_CHAIRS 3 // Number of chairs in the barbershop

pthread_mutex_t mutex; // Mutex for critical section
pthread_cond_t barberReady; // Condition variable for barber availability
pthread_cond_t customerReady; // Condition variable for customer readiness
int waitingCustomers = 0; // Number of waiting customers

void* barber(void* arg) {
    while (1) {
        // Wait for a customer to be ready
        pthread_mutex_lock(&mutex); // Entry to critical section (lock)
        while (waitingCustomers == 0) {
            pthread_cond_wait(&customerReady, &mutex); // Wait for a customer
        }
        waitingCustomers--; // A customer is being served
        pthread_mutex_unlock(&mutex); // Exit from critical section (unlock)
        
        // Critical section (serving the customer)
        printf("Barber is cutting hair.\n");
        sleep(rand() % 3); // Simulate haircut
        pthread_cond_signal(&barberReady); // Signal that barber is ready for next customer
    }
    return NULL;
}

void* customer(void* num) {
    int id = *(int*)num;
    
    pthread_mutex_lock(&mutex); // Entry to critical section (lock)
    if (waitingCustomers < NUM_CHAIRS) {
        waitingCustomers++; // A customer takes a seat
        printf("Customer %d is waiting.\n", id);
        pthread_cond_signal(&customerReady); // Signal that a customer is ready
        pthread_mutex_unlock(&mutex); // Exit from critical section (unlock)

        pthread_cond_wait(&barberReady, &mutex); // Wait for the barber to be ready
        printf("Customer %d is getting a haircut.\n", id);
    } else {
        printf("Customer %d left (no seats available).\n", id);
        pthread_mutex_unlock(&mutex); // Exit from critical section (unlock)
    }
    return NULL;
}

int main() {
    pthread_t barberThread;
    pthread_t customers[10]; // Up to 10 customers
    int customerIds[10];

    // Initialize mutex and condition variables
    pthread_mutex_init(&mutex, NULL); // Initialize mutex
    pthread_cond_init(&barberReady, NULL); // Initialize barber ready condition
    pthread_cond_init(&customerReady, NULL); // Initialize customer ready condition

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

    // Clean up mutex and condition variables (not reached in this case)
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&barberReady);
    pthread_cond_destroy(&customerReady);

    return 0;
}

