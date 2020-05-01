#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<pthread.h>
#include<semaphore.h>

sem_t full;
sem_t empty;
sem_t countH;
sem_t either;
int count;
int size=5;
int positionEnq=0,positionDeq=0;
int bufferFlag=0;
int n,i,tempC,tempP;


typedef struct
{
    int data;
    struct Node*next;
} Node;

typedef struct
{
    Node *front, *rear;

} Queue;


Node*newNode(int val)
{
    Node *n= (Node*) malloc(sizeof(Node));
    n->data=val;
    n->next=NULL;
    return n;
}

void initializeQ(Queue *q)
{
    q->front=q->rear=NULL;
}

void enqueue(Queue *q,int val)
{
    Node *ptr=newNode(val);
    if(q->front==NULL)
        q->front=q->rear=ptr;
    else
    {
        q->rear->next=ptr;
        q->rear=ptr;
    }
    positionEnq++;
}

int dequeue(Queue *q)
{
    int x;
    positionDeq++;
    if(q->front==NULL)
        exit(1);
    else
    {
        x=q->front->data;
        q->front=q->front->next;
        return x;
    }

}


Queue *myBuffer;


void *messageHandling(void* a)
{
    int c=(int ) a;
    while(1)
    {
        int time2= (rand() % 15)+1;
        sleep(time2);
        printf("Counter thread %d:waiting to write\n\n",c);
        sem_wait(&countH);
        printf("Counter thread %d:recieved a message\n\n", c);
        count++;
        printf("Counter thread %d:now adding to counter, counter value=%d\n\n",c,count);
        sem_post(&countH);
    }
}
void *consumer_fn(void* a)
{
    while(1)
    {
        if(bufferFlag==0)
        {
            printf("Collector thread: nothing is in buffer!\n\n");
        }
        int time= (rand() %40)+10;
        sleep(time);
        sem_wait(&empty);
        sem_wait(&either);
        int y;
        y=dequeue(myBuffer);
        tempC= positionDeq%size;
        printf("Collector thread: Writing from buffer at position %d\n\n", tempC);
        bufferFlag--;
        sem_post(&full);
        sem_post(&either);


    }
}

void *producer_fn(void* a)
{
    while(1)
    {

        if(bufferFlag==size)
        {
            printf("Monitor thread: Buffer full!\n\n");
        }

        printf("Monitor thread:waiting to read counter\n\n");
        int time3= (rand() % 15)+1;
        sleep(time3);
        sem_wait(&full);
        sem_wait(&either);
        sem_wait(&countH);

        enqueue(myBuffer, count);
        printf("Monitor thread: reading a count value of %d\n\n", count);
        tempP=positionEnq%size;
        printf("Monitor thread: writing to buffer at position %d\n\n", tempP);
        bufferFlag++;
        count=0;

        sem_post(&empty);
        sem_post(&either);
        sem_post(&countH);

    }
}


int main()
{
    sem_init(&full,0,size);
    sem_init(&empty,0,0);
    sem_init(&countH,0,1);
    sem_init(&either,0,1);

    myBuffer=malloc(sizeof(Queue));
    initializeQ(myBuffer);


    int msgsNum=10;
    pthread_t *msgsThreads=(pthread_t *)malloc(msgsNum*sizeof(pthread_t));


    for(i=0; i<msgsNum; i++)
    {
        pthread_create(&msgsThreads[i],NULL,messageHandling,(void*) i);
    }

    pthread_t monThread;
    pthread_t colThread;

    pthread_create(&monThread, NULL, producer_fn,NULL);
    pthread_create(&colThread, NULL, consumer_fn,NULL);

    for(n=0; n<msgsNum; n++)

    {
        pthread_join(msgsThreads[n],NULL);
    }

    pthread_join(monThread, NULL);
    pthread_join(colThread, NULL);


    return 0;

}
