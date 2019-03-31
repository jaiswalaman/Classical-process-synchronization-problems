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


typedef int buffer_item;
#define BUFFER_SIZE 5


buffer_item buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t full, empty;
int count, in, out;

// Function prototypes
int insertItem(buffer_item item);
int removeItem(buffer_item *item);
void *consumer(void *param);
void *producer(void *param);

int main(){
 
  
 int stime;
 int numOfproducer;
  int numOfconsumer;
   printf("enter sleep time  ");
   scanf("%d",&stime);
   printf("enter numOfproducer  ");
   scanf("%d",&numOfproducer);
   printf("enter numOfconsumer  ");
   scanf("%d",&numOfconsumer);
  // Initialize
  int i;
  srand(time(NULL));
  pthread_mutex_init(&mutex, NULL);
  sem_init(&empty, 0, BUFFER_SIZE); // All of buffer is empty
  sem_init(&full, 0, 0);
  count = in = out = 0;

  // Create the producer and consumer threads
  pthread_t producers[numOfproducer];
  pthread_t consumers[numOfconsumer];
  for(i = 0; i < numOfproducer; i++)
    pthread_create(&producers[i], NULL, producer, NULL);
  for(i = 0; i < numOfconsumer; i++)
    pthread_create(&consumers[i], NULL, consumer, NULL);

  // Sleep before terminating
  sleep(stime);
  return 0;
}

// Insert item into buffer.
//Returns 0 if successful, -1 indicating error
int insertItem(buffer_item item){
  int success;
  sem_wait(&empty);
  pthread_mutex_lock(&mutex);

  // Add item to buffer
  if( count != BUFFER_SIZE){
    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;
    count++;
    success = 0;
  }
  else
    success = -1;

  pthread_mutex_unlock(&mutex);
  sem_post(&full);
  
  return success;
}

// Remove an object from the buffer, placing it in item.
// Returns 0 if successful, -1 indicating errorv
int removeItem(buffer_item *item){
  int success;
  
  sem_wait(&full);
  pthread_mutex_lock(&mutex);
  
  // Remove item from buffer to item
  if( count != 0){
    *item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    count--;
    success = 0;
  }
  else
    success = -1;

  pthread_mutex_unlock(&mutex);
  sem_post(&empty);
  
  return success;
}

void *producer(void *param){
  buffer_item item;
  while(1){
    sleep(rand() % 7 + 1); // Sleep randomly between 1 and 7 seconds
    
    item = rand();
    if(insertItem(item))
      printf("Error\n");
    else
      printf("Producer has produced %d\n", item);
  }
}


void *consumer(void *param){
  buffer_item item;
  while(1){
    sleep(rand() % 7 + 1); // Sleep randomly between 1 and 7 seconds

    if(removeItem(&item))
      printf("Error occured\n");
    else
      printf("Consumer has consumed %d\n", item);
  }
}
