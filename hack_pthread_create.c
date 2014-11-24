/*
$gcc -g -lpthread -Wall -o hack_pthread_create hack_pthread_create.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
void* thread(void* arg)
{
	printf("This is a pthread.\n");

	sleep(5);

	return((void *)0);
}

int main(int argc,char **argv)
{
	pthread_t id;
	int ret;

	printf("pthread_start\n");
	ret = pthread_create(&id,NULL,thread,NULL);
	printf("pthread_end\n");
	if(ret != 0)
	{
		printf("Create pthread error!\n");
		exit(1);
	}
	printf("This is the main process.\n");

	pthread_join(id,NULL);

	return 0;
}
