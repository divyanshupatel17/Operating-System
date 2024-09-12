#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *credit(void *s);
void *debit(void *s);
int flag[2];
int turn;
long int balance = 0;

void lock_init()
{
    flag[0] = flag[1] = 0;
    turn = 0;
}

void lock(int self)
{
    flag[self] = 1;
    turn = 1 - self;
    while (flag[1 - self] == 1 && turn == 1 - self);
}

void unlock(int self)
{
    flag[self] = 0;
}

void *credit(void *s)
{
    int self = 0; // Father's thread (ie credit 1000)
    lock(self);
    balance += 1000;
    printf("Father credited: 1000\n");
    printf("Current balance: %ld\n", balance);
    unlock(self);
    return NULL;
}

void *debit(void *s)
{
    int self = 1; // Brother's thread (ie debit 200)
    lock(self);
    if (balance >= 200) {
        balance -= 200;
        printf("Brother debited: 200\n");
    } else {
        printf("Insufficient funds for debit\n");
    }
    printf("Current balance: %ld\n", balance);
    unlock(self);
    return NULL;
}

int main()
{
    pthread_t t1, t2;
    
    lock_init();

    pthread_create(&t1, NULL, credit, NULL);
    pthread_create(&t2, NULL, debit, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final Balance: %ld\n", balance);

    return 0;
}







=