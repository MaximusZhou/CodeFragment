
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>


void *func(void *param) {
	static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

	 pthread_mutex_lock(&mutex);
	 sleep(10);
	 pthread_mutex_unlock(&mutex);

	 return 0;
}
int main(int argc, char* argv[]) {
	pthread_t tid;
	int res;

	pthread_create(&tid, NULL, func, NULL);

	if(fork() == 0){
		func(0);
		return 0;
	}

	res = pthread_join(tid, 0);
	printf("Res:%d\n",res);

	return 0;
}
