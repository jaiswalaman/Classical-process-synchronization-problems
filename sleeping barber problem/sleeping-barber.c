//Aman Jaiswal ,B.Tech CSE I.I.T Roorkee
//enrollment No:-17114008

#define MAX 20
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
//#include <semaphore.h> inbuilt method for semaphores
#include "SemaphoreCustom.h"

void *customer(void *param);
void *barber(void *param);

sem_t chairs_mutex;
sem_t sem_customer;
sem_t sem_barber;
int numberChairs;
int customerWait;
int main() {
	pthread_t barberid;
	pthread_t customerids[MAX];
	
	printf("Main thread beginning\n");
   /* 1. Get command line arguments */
   int runTime,customers,i;
   printf("enter run time  ");
   scanf("%d",&runTime);
   printf("enter num Of customers  ");
   scanf("%d",&customers);
   printf("enter number of Chairs  ");
   scanf("%d",&numberChairs);
   printf("enter wait time for next customer");
   scanf("%d",&customerWait);
  
   /* 2. Initialize semaphores */
   sem_init(&chairs_mutex,0,1);
   sem_init(&sem_customer,0,0);
   sem_init(&sem_barber,0,0);
   /* 3. Create barber thread. */
   pthread_create(&barberid, NULL, barber, NULL);
   printf("Creating barber  with id %lu\n",barberid);
   /* 4. Create customer threads.  */
   for (i = 0; i < customers; i++){
	   pthread_create(&customerids[i], NULL, customer, NULL);
	   printf("Creating customer  with id %lu\n",customerids[i]);
   }
   /* 5. Sleep. */
   printf("Main  sleeping for %d seconds\n", runTime);
   sleep(runTime);
   /* 6. Exit.  */
   printf("Main  exiting\n");
   exit(0);
}

void *barber(void *param) {
   int worktime;
  
   while(1) {
      /* wait for a customer to become available (sem_customer) */
	  sem_wait(&sem_customer);
      /* wait for mutex to access chair count (chair_mutex) */
	  sem_wait(&chairs_mutex);
      /* increment number of chairs available */
	  numberChairs += 1;
	  printf("Barber: Taking a customer. Number of chairs available = %d\n",numberChairs);
      /* signal to customer that barber is ready to cut their hair (sem_barber) */
	  sem_post(&sem_barber);
      /* give up lock on chair count */
	  sem_post(&chairs_mutex);
      /* generate random number, worktime, from 1-4 seconds for length of haircut.  */
	  worktime = (rand() % 4) + 1;
      /* cut hair for worktime seconds (really just call sleep()) */
	  printf("Barber: Cutting hair for %d seconds\n", worktime);
	  sleep(worktime);
    } 
}

void *customer(void *param) {
   int waittime;

   while(1) {
      /* wait for mutex to access chair count (chair_mutex) */
	  sem_wait(&chairs_mutex);
      /* if there are no chairs */
	  if(numberChairs <= 0){
           /* free mutex lock on chair count */
		   printf("customer:  %u leaving with no haircut\n", (unsigned int)pthread_self());
		   sem_post(&chairs_mutex);
	  }
      /* else if there are chairs */
	  else{
           /* decrement number of chairs available */
		   numberChairs -= 1;
		   printf("customer:  %u Sitting to wait. Number of chairs left = %d\n",(unsigned int)pthread_self(),numberChairs);
           /* signal that a customer is ready (sem_customer) */
		   sem_post(&sem_customer);
           /* free mutex lock on chair count */
		   sem_post(&chairs_mutex);
           /* wait for barber (sem_barber) */
		   sem_wait(&sem_barber);
           /* get haircut */
		   printf("customer:  %u getting a haircut\n",(unsigned int)pthread_self());
	  }
      /* generate random number, waittime, for length of wait until next haircut or next try.  Max value from command line. */
	  waittime = (rand() % customerWait) + 1;
      /* sleep for waittime seconds */
	  printf("customer:  %u waiting %d seconds before attempting next haircut\n",(unsigned int)pthread_self(),waittime);
	  sleep(waittime);
     }
}
