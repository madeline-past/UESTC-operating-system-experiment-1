#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#define buffersize 10
#define numofproducer 3
#define numofconsumer 4
char buffer[buffersize];
int in=0,out=0; 
pthread_mutex_t mutex;
sem_t empty,full;
FILE * fp;

void producer(void){
    int c;
    while ((c = fgetc(fp)) != EOF) {
        sem_wait(&empty);//P
        pthread_mutex_lock(&mutex);
        pthread_t producer;
        producer=pthread_self();
        printf("编号%lu的生产者线程:",producer);
        printf("%c\n",c);
        buffer[in]=c;
        in= (in+1)%buffersize;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);//V
     }
    pthread_exit(NULL);
}


void consumer(void){
    int num;
    sem_getvalue(&full,&num);
    while(num!=0){
        sem_wait(&full);//P
        pthread_mutex_lock(&mutex);
        pthread_t consumer;
        consumer=pthread_self();
        printf("编号%lu的消费者线程:",consumer);
        printf("%c\n",buffer[out]);
        out=(out+1)%buffersize;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);//V
        sem_getvalue(&full,&num);
    }
    pthread_exit(NULL);
}


int main()
{
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0,buffersize);
    sem_init(&full,0,0);
    int i;
    fp = fopen ("file.txt", "r");
    pthread_t producer_thread[numofproducer];
    pthread_t consumer_thread[numofconsumer];
    for (i = 0; i < numofproducer; i++) {
        pthread_create(&producer_thread[i], NULL, (void *) producer, NULL);
    }
    for (i = 0; i < numofconsumer; i++) {
        pthread_create(&consumer_thread[i], NULL, (void *) consumer, NULL);
    }
    for (i = 0; i < numofproducer; i++) {
        pthread_join(producer_thread[i], NULL);
    }
    for (i = 0; i < numofconsumer; i++) {
        pthread_join(consumer_thread[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}

