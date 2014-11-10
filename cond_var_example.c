/*
$gcc -Wall -lpthread cond_var_example.c  -o cond_var_example
$./cond_var_example  1000000 5
count[0] = 220234
count[1] = 201652
count[2] = 199165
count[3] = 182972
count[4] = 195977
nsignals = 3
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define	MAXNITEMS 		1000000
#define	MAXNTHREADS			100
#define MIN(a,b) (((a) < (b))?(a):(b))

int		nitems;				/* read-only by producer and consumer,to express maximum item */
int		buff[MAXNITEMS];
struct {
  pthread_mutex_t	mutex;
  int				nput;	/* next index to store */
  int				nval;	/* next value to store */
} put = { PTHREAD_MUTEX_INITIALIZER };

struct {
  pthread_mutex_t	mutex;
  pthread_cond_t	cond;
  int				nready;	/* number ready for consumer */
} nready = { PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER };

int		nsignals; /*call pthread_cond_signal count*/

void	*produce(void *), *consume(void *);

int main(int argc, char **argv)
{
	int			i, nthreads, count[MAXNTHREADS];
	pthread_t	tid_produce[MAXNTHREADS], tid_consume;

	if (argc != 3)
	{
		printf("usage: cond_var_example <#items> <#threads>\n");
		return 1;
	}
	nitems = MIN(atoi(argv[1]), MAXNITEMS);
	nthreads = MIN(atoi(argv[2]), MAXNTHREADS);

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
	printf("nsignals = %d\n", nsignals);

	exit(0);
}

void * produce(void *arg)
{
	for ( ; ; ) {
		pthread_mutex_lock(&put.mutex);
		if (put.nput >= nitems) {
			pthread_mutex_unlock(&put.mutex);
			return(NULL);		/* array is full, we're done */
		}
		buff[put.nput] = put.nval;
		put.nput++;
		put.nval++;
		pthread_mutex_unlock(&put.mutex);

		pthread_mutex_lock(&nready.mutex);
		if (nready.nready == 0) {
			pthread_cond_signal(&nready.cond);
			nsignals++;
		}
		nready.nready++;
		pthread_mutex_unlock(&nready.mutex);

		*((int *) arg) += 1;
	}
}

void * consume(void *arg)
{
	int		i;

	for (i = 0; i < nitems; i++) {
		pthread_mutex_lock(&nready.mutex);
		while (nready.nready == 0)
			pthread_cond_wait(&nready.cond, &nready.mutex);
		nready.nready--;
		pthread_mutex_unlock(&nready.mutex);

		if (buff[i] != i)
			printf("buff[%d] = %d\n", i, buff[i]);
	}
	return(NULL);
}
