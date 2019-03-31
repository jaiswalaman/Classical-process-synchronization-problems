//Aman Jaiswal ,B.Tech CSE I.I.T Roorkee
//enrollment No:-17114008


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
//#include <semaphore.h>//inbuilt method for semaphores
#include "SemaphoreCustom.h"
#include <time.h>

sem_t readCountAccess;
sem_t dataAccess;
int readCount=0;

void *Reader(void *arg);
void *Writer(void *arg);

int main()
{
 int i=0,NoOfReader=0,NoOfWriter;
 sem_init(&readCountAccess,0,1);
 sem_init(&dataAccess,0,1);
 
 pthread_t Readers_thr[100],Writer_thr[100];
 printf("\nEnter no of Readers");
 scanf("%d",&NoOfReader); 
 printf("\nEnter no of Writers");
 scanf("%d",&NoOfWriter); 
 
 for(i=0;i<NoOfReader;i++)
 {
  pthread_create(&Readers_thr[i],NULL,Reader,(void *)i);
 }
 for(i=0;i<NoOfWriter;i++)
 { 
  pthread_create(&Writer_thr[i],NULL,Writer,(void *)i);
 }
 for(i=0;i<NoOfWriter;i++)
 {
  pthread_join(Writer_thr[i],NULL);
 }

 for(i=0;i<NoOfReader;i++)
 {
  pthread_join(Readers_thr[i],NULL); 
 }
 sem_destroy(&dataAccess);
 sem_destroy(&readCountAccess); 
 return 0;
}

void * Writer(void *arg)
{
 
 sleep(1); 
 int temp=(int)arg;
 printf("\nWriter %d is trying to write the data",temp);
 sem_wait(&dataAccess);
 printf("\nWriter %d is writting the data",temp); 
 printf("\nWriter %d wrote the data"); 
 sem_post(&dataAccess);
}

void *Reader(void *arg)
{ 
 sleep(1); 
 int temp=(int)arg;
 printf("\nReader %d is trying to read the data",temp);
 sem_wait(&readCountAccess);
 readCount++;
 if(readCount==1)
 {
  sem_wait(&dataAccess);
  printf("\nReader %d is reading the data",temp); 
 }
 sem_post(&readCountAccess);
 sem_wait(&readCountAccess);
 readCount--;
 if(readCount==0)
 {
  printf("\nReader %d read the data",temp);  
  sem_post(&dataAccess);
 }
 sem_post(&readCountAccess);
}
