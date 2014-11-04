/*
	$gcc -Wall -std=c99 test_pthread_fork.c  -lpthread -o fork_in_thread

*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void *func(void *param) {
	sleep(10000);
	return((void *)0);
}

void create_threads(int n) {
	pthread_t t;
	for (int i = 0; i < n; i++)
		pthread_create(&t, 0, func, 0);
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("usage: %s thread_num\n", argv[0]);
		return 0;
	}
	create_threads(atoi(argv[1]));
	fork();
	sleep(10000);
	return 0;
}
