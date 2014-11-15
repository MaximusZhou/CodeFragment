/*
$gcc -o sem_example -Wall -lpthread  sem_example.c 
$./sem_example 
[P1] Producing item0 ...
[P0] Producing item0 ...
[P0] Producing item1 ...
[P2] Producing item0 ...
[P2] Producing item1 ...
------> [C1] Consuming item0 ...
------> [C1] Consuming item0 ...
------> [C2] Consuming item1 ...
------> [C2] Consuming item0 ...
------> [C0] Consuming item1 ...
[P1] Producing item1 ...
------> [C0] Consuming item1 ...
[P0] Producing item2 ...
[P0] Producing item3 ...
------> [C1] Consuming item2 ...
------> [C1] Consuming item3 ...
[P1] Producing item2 ...
[P1] Producing item3 ...
[P2] Producing item2 ...
[P2] Producing item3 ...
------> [C0] Consuming item2 ...
------> [C0] Consuming item3 ...
------> [C2] Consuming item2 ...
------> [C2] Consuming item3 ...
*/
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define BUFF_SIZE   5/* total number of slots */
#define NP          3/* total number of producers */
#define NC          3/* total number of consumers */
#define NITERS      4/* number of items produced/consumed */
#define NONITEM     -1/* stand for no item*/

typedef struct {
	int buf[BUFF_SIZE];   /* shared var */
	int in;         	  /* buf[in%BUFF_SIZE] is the first empty slot */
	int out;        	  /* buf[out%BUFF_SIZE] is the first full slot */
	sem_t full;     	  /* keep track of the number of full spots */
	sem_t empty;    	  /* keep track of the number of empty spots */
	sem_t mutex;    	  /* enforce mutual exclusion to shared data */
} sbuf_t;

sbuf_t shared;

void *producer(void *arg)
{
	int i, item, index;

	index = (int)arg;

	for (i=0; i < NITERS; i++) 
	{

		/* Produce item */
		item = i;

		/* Prepare to write item to buf */

		/* If there are no empty slots, wait */
		sem_wait(&shared.empty);

		/* If another thread uses the buffer, wait */
		sem_wait(&shared.mutex);
		shared.buf[shared.in] = item;
		shared.in = (shared.in+1)%BUFF_SIZE;
		printf("[P%d] Producing item%d ...\n", index, item); 
		fflush(stdout);
		/* Release the buffer */
		sem_post(&shared.mutex);

		/* Increment the number of full slots */
		sem_post(&shared.full);

		/* Interleave producer and consumer execution */
		if (i % 2 == 1) 
			sleep(1);
	}
	return NULL;
}

void *consumer(void *arg)
{
	int i, item, index;

	index = (int)arg;

	for (i=0; i < NITERS; i++) 
	{

		/* Prepare to read item to buf */
		/* If there are no full slots, wait */
		sem_wait(&shared.full);

		/* If another thread uses the buffer, wait */
		sem_wait(&shared.mutex);/* consume item */
		item = shared.buf[shared.out];
		shared.buf[shared.in] = NONITEM;
		shared.out = (shared.out+1)%BUFF_SIZE;
		printf(" ------> [C%d] Consuming item%d ...\n", index, item); 
		fflush(stdout);
		sem_post(&shared.mutex);/* Release the buffer */

		/* Increment the number of empty slots */
		sem_post(&shared.empty);

		/* Interleave producer and consumer execution */
		if (i % 2 == 1) 
			sleep(1);
	}
	return NULL;
}

int main()
{
	pthread_t idP[NP], idC[NC];
	int index;

	/*initialize an unnamed semaphore*/
	sem_init(&shared.full, 0, 0);
	sem_init(&shared.empty, 0, BUFF_SIZE);

	/*initialize mutex*/
	sem_init(&shared.mutex, 0, 1);

	/* Create NP producer */
	for (index = 0; index < NP; index++)
	{  
		pthread_create(&idP[index], NULL, producer, (void*)index);
	}

	/*Create NC consumers */
	for (index = 0; index < NC; index++)
	{  
		pthread_create(&idC[index], NULL, consumer, (void*)index);
	}

	/* wait for all producers and the consumer */
	for (index = 0; index < NP; index++)
	{  
		pthread_join(idP[index], NULL);
	}
	for (index = 0; index < NC; index++)
	{  
		pthread_join(idC[index], NULL);
	}

	exit(0);
}
