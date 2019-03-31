//Aman Jaiswal ,B.Tech CSE I.I.T Roorkee
//enrollment No:-17114008
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
//#include <semaphore.h>//inbuilt method for semaphores
#include "SemaphoreCustom.h"

#define SHARED 0
#define PHILOSOPHERS 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2

sem_t sem[PHILOSOPHERS];
pthread_mutex_t mutex;
int state[PHILOSOPHERS];
void philosopher( int n );
void getForks( int n );
void putForks( int n );
void test( int n );
int main( void ) {

	pthread_t tid[PHILOSOPHERS];
	for(int i = 0; i < PHILOSOPHERS; i++){
		sem_init(&sem[i], SHARED, 1);
	}

	printf("Starting threads\n");

	for(int i = 0; i < PHILOSOPHERS; i++){
		pthread_create(&tid[i], NULL, philosopher, i);
	}

	printf("Closing threads\n");  // Programflow will not come here...

	for(int i = 0; i < PHILOSOPHERS; i++){
		pthread_join(&tid[i], NULL);
	}

	return 0;

}

void philosopher( int n ) {
	
	while(1) {
		printf("Thinking, %d\n", n);	
		usleep( rand() % 10000 );  // Thinking
		printf("Time to eat, %d\n", n);	
		getForks( n );
		usleep( rand() % 1000 );   // Eating
		printf("Done eating, %d\n", n);
		putForks( n );
	}
}

void getForks( int n ) {

	pthread_mutex_lock( &mutex );
	state[n] = HUNGRY;
	test( n );
	pthread_mutex_unlock( &mutex );
	sem_wait(&sem[n]);
}

void putForks( int n ) {

	pthread_mutex_lock( &mutex );
	state[n] = THINKING;
	test( (n + PHILOSOPHERS - 1) % PHILOSOPHERS );
	test( (n + 1) % PHILOSOPHERS );
	pthread_mutex_unlock( &mutex );
}

void test( int n ) { 

	if(state[n] == HUNGRY && state[(n + PHILOSOPHERS - 1) % PHILOSOPHERS] != EATING && state[(n + 1) % PHILOSOPHERS] != EATING) {
	       state[n] = EATING;
	       sem_post(&sem[n]);
	}
}
