#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define size 5

int buffer[size];
sem_t s, n, e,c;
int count = 0;

int incomingMessages = 20;
int receivedMessages = 0;

void* counter(void* args)
{
    int num = *((int*)args);
    
    sleep(rand() % (20 - 1 + 1) + 1);
    
    printf("\nCounter Thread %d: received a message\n", num);
    
    if(sem_wait(&c)) {
        printf("\nCounter Thread %d: waiting to write\n", num);
    }
    count++;
    printf("\nCounter Thread %d: now adding to counter, counter value = %d\n", num, count);
    sem_post(&c);
}

void* monitor(void* args)
{
    int i = 0;
    
    while(receivedMessages < incomingMessages)
    {
        int readCount;
        
        if(sem_wait(&c)) {
            printf("\nMonitor Thread: waiting to read counter\n");
        }
        readCount = count;
        count = 0;
        printf("\nMonitor Thread: reading a count value of %d\n", readCount);
        sem_post(&c);
        
        if(sem_wait(&e)) {
            printf("\nMonitor Thread: buffer is full\n");
        }
        if(sem_wait(&s)) {
            printf("\nMonitor Thread: waiting to write to buffer\n");
        }
        printf("\nMonitor Thread: writing to buffer at position %d\n", i);
        buffer[i] = readCount;
        sem_post(&s);
        sem_post(&n);
        
        i++;
        if(i >= size) i = 0;
        
        sleep(rand() % (3 - 2 + 1) + 2);
    }
}

void* collector(void* args)
{
    int i = 0;
    
    while(receivedMessages < incomingMessages)
    {
        if(sem_wait(&n)) {
            printf("\nCollector Thread: buffer is empty\n");
        }
        if(sem_wait(&s)) {
            printf("\nCollector Thread: waiting to read from buffer\n");
        }
        printf("\nCollector Thread: reading from buffer at position %d, value = %d\n", i, buffer[i]);
        receivedMessages = receivedMessages + buffer[i];
        sem_post(&s);
        sem_post(&e);
        
        i++;
        if(i >= size) i = 0;
        
        sleep(rand() % (3 - 2 + 1) + 2);
    }
}

void initializeSemaphores()
{
    sem_init(&e, 0, size);
    sem_init(&n, 0, 0);
    sem_init(&s, 0, 1);
    
    sem_init(&c, 0, 1);
}

int main()
{
    srand(time(0));
    
    initializeSemaphores();
    
    pthread_t counterThreads[incomingMessages];
    int indices[incomingMessages];
    
    int i = 0;
    while(i < incomingMessages)
    {
        indices[i] = i;
        pthread_create(&counterThreads[i], NULL, counter, &indices[i]);
        i++;
    }
    
    pthread_t monitorThread, collectorThread;
    
    pthread_create(&monitorThread, NULL, monitor, NULL);
    pthread_create(&collectorThread, NULL, collector, NULL);
    
    i = 0;
    while(i < incomingMessages)
    {
        pthread_join(counterThreads[i++], NULL);
    }
    
    pthread_join(monitorThread, NULL);
    pthread_join(collectorThread, NULL);
    
    return 0;
}
