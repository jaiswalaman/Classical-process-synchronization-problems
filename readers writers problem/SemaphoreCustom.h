//Aman Jaiswal ,B.Tech CSE I.I.T Roorkee
//enrollment No:-17114008


#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h> 

typedef struct 
{
  pthread_cond_t* value;
  struct node* next;
}node; // struct for each element in qeueue of a semaphore of suspended threads

typedef struct Queue
{
        pthread_mutex_t QueueMutex;
        int size;
        node* front;
        node* rear;
        int *elements;
}Queue;

typedef struct
{
   atomic_int s;  // semaphore atomic varible
   Queue Q;           // queue for storing suspended threads
   pthread_mutex_t Spinlock1; 
   pthread_mutex_t Spinlock2;
  } sem_t; //struct for semaphore
 
int enqueue(struct Queue *q, pthread_cond_t* con)
{
	
 pthread_mutex_lock(&q->QueueMutex);
	node* temp = (node *)malloc(sizeof(node));

	

	temp->value= con;
	temp->next = NULL;

	if (q->front == NULL) {
		q->front = temp;
		q->rear = temp;
		q->size = 0;
	}else{
	q->rear->next = temp;
	q->rear = temp;
	}
q->size += 1;
  pthread_mutex_unlock(&q->QueueMutex);
	
}// insert suspended threads in queue




pthread_cond_t* dequeue(struct Queue *q)
{
	pthread_mutex_lock(&q->QueueMutex);
	if (q->size <= 0) {
		pthread_mutex_unlock(&q->QueueMutex);
		return NULL;
	}else{


pthread_cond_t* releasedSem=q->front->value;
	
if(q->size==1)
	 	{
	 		q->front=NULL;
	 		q->rear=NULL;
	 		q->size=0;
	 	}
	 	else
	 	{
	 		q->size--;
      q->front=q->front->next;
	 	}
    pthread_mutex_unlock(&q->QueueMutex);
	  
   	return releasedSem;
	 
   }
	 

	return NULL;
}// remove element from queue for suspended threads




//wait(S)
 void sem_wait(sem_t* temp) 
 {
 	pthread_mutex_lock(&temp->Spinlock1);  
    temp->s--;
    if(temp->s<0)
    {
      pthread_cond_t* cond_temp=(pthread_cond_t*)malloc(sizeof(pthread_cond_t));
      enqueue(&temp->Q,cond_temp); 
      pthread_cond_wait(cond_temp,&temp->Spinlock1); 
 } 

    pthread_mutex_unlock(&temp->Spinlock1);

}
//signal(S)
 void sem_post(sem_t* temp)
 {
  pthread_mutex_lock(&temp->Spinlock2); 

    temp->s++;    
    if(temp->s<=0)
    {
      
    	pthread_cond_t* cond=dequeue(&temp->Q);
    	if(cond!=NULL)
      {
      pthread_cond_signal(cond);   
      }

    }
  pthread_mutex_unlock(&temp->Spinlock2);

 }
void sem_init( sem_t* temp,int pshared,unsigned int valueue) // semaphore inilization
 {
    temp->s=valueue;
  
 }

