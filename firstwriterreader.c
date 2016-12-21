// src: IIIT Hyderabad, Operating Systems 2011.
#include<stdio.h>
#include<sys/types.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>

sem_t wrt,read,mutex;
int wrtcount=0;
char data[100] = "";

void *readThread(void *param){
    sem_wait(&read);
    printf("Reader Thread %ld, Data read is %s\n", (long)param,data);
    sem_post(&read);
}

void *writeThread(void *param){
    sem_wait(&mutex);
    wrtcount++;
    if(wrtcount==1)
	sem_wait(&read);
    sem_post(&mutex);

    sem_wait(&wrt);
    strcat(data, "hi ");
    printf("Writer thread %ld Data written: hi\n", (long)param);
    sem_post(&wrt);

    sem_wait(&mutex);
    wrtcount--;
    if(wrtcount ==0)
	sem_post(&read);
    sem_post(&mutex);
}

int main(){
    sem_init(&wrt,0,1);
    sem_init(&read,0,1);
    sem_init(&mutex,0,1);
    int i;
    pthread_t rthread[5];
    pthread_t wthread[5];

    for(i=0;i<5;i++)
    {
    	pthread_create(&wthread[i],NULL,writeThread,(void *)i);
    }
    for(i=0;i<5;i++)
    {
    	pthread_create(&rthread[i],NULL,readThread,(void *)i);
    }
    for(i=0;i<5;i++)
	pthread_join(rthread[i],NULL);
    for(i=0;i<5;i++)
	pthread_join(wthread[i],NULL);

    printf("Final string is %s\n",data);
    pthread_exit(NULL);
    return 0;
}
