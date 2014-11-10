/*
$gcc -Wall -lpthread mutex_example.c -o mutex_example
$./mutex_example 1000000 5
count[0] = 188090
count[1] = 197868
count[2] = 194924
count[3] = 211562
count[4] = 207556
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define	MAXNITEMS 		1000000
#define	MAXNTHREADS			100
#define MIN(a,b) (((a) < (b))?(a):(b))

int		nitems;			/* read-only by producer and consumer,to express maximum item */
struct {
  pthread_mutex_t	mutex;
  int	buff[MAXNITEMS];
  int	nput;
  int	nval;
} shared = { PTHREAD_MUTEX_INITIALIZER };

void	*produce(void *), *consume(void *);

int main(int argc, char **argv)
{
	int			i, nthreads, count[MAXNTHREADS];
	pthread_t	tid_produce[MAXNTHREADS], tid_consume;

	if (argc != 3)
	{
		printf("usage: prodcons4 <#items> <#threads>\n");
		return 1;
	}
	nitems = MIN(atoi(argv[1]), MAXNITEMS);
	nthreads = MIN(atoi(argv[2]), MAXNTHREADS);

	printf("main:%d,%d,%d",shared.nput,shared.nval,shared.buff[0]);

	/* create all producers and one consumer */
	for (i = 0; i < nthreads; i++) {
		count[i] = 0;
		pthread_create(&tid_produce[i], NULL, produce, &count[i]);
	}
	pthread_create(&tid_consume, NULL, consume, NULL);

	/* wait for all producers and the consumer */
	for (i = 0; i < nthreads; i++) {
		pthread_join(tid_produce[i], NULL);
		printf("count[%d] = %d\n", i, count[i]);	
	}
	pthread_join(tid_consume, NULL);

	exit(0);
}

void * produce(void *arg)
{
	for ( ; ; ) {
		pthread_mutex_lock(&shared.mutex);
		if (shared.nput >= nitems) {
			pthread_mutex_unlock(&shared.mutex);
			return(NULL);		/* array is full, we're done */
		}
		shared.buff[shared.nput] = shared.nval;
		shared.nput++;
		shared.nval++;
		pthread_mutex_unlock(&shared.mutex);
		*((int *) arg) += 1;
	}
}

void consume_wait(int i)
{
	for ( ; ; ) {
		pthread_mutex_lock(&shared.mutex);
		if (i < shared.nput) {
			pthread_mutex_unlock(&shared.mutex);
			return;			/* an item is ready */
		}
		pthread_mutex_unlock(&shared.mutex);
		sched_yield();
	}
}

void * consume(void *arg)
{
	int		i;

	for (i = 0; i < nitems; i++) {
		consume_wait(i);
		if (shared.buff[i] != i)
			printf("buff[%d] = %d\n", i, shared.buff[i]);
	}
	return(NULL);
}
