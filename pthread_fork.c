/*
	URL:http://hi.baidu.com/_kouu/item/358716f4c5f0cd0ec6dc45d0
	$gcc -Wall -std=c99 pthread_fork.c  -lpthread -o pthread_fork


	while comment code create_threads(atoi(argv[2]));
	$./pthread_fork  2 1 1
	$ps axo pid,ppid,comm | grep pthread_fork
	59826 59728 pthread_fork
	59832 59826 pthread_fork
	$ls /proc/59826/task/ | wc -l   
	3
	$ls /proc/59832/task/ | wc -l     
	1

	while not comment code create_threads(atoi(argv[2]));
*/


#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <linux/unistd.h>
#include <sys/syscall.h>

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
	if (argc != 4) {
		printf("usage: %s (parent_thread_num) (child_thread_num) (is_direct_fork)\n", argv[0]);
		return 0;
	}
	create_threads(atoi(argv[1]));
	if (!(*argv[3] == '1' ? syscall(__NR_fork) : fork())) {
		sleep(10);
		create_threads(atoi(argv[2]));
	}
	sleep(10000);
	return 0;
}
