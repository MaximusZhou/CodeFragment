#include <stdio.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <assert.h>

#define gettid() syscall(__NR_gettid)

pthread_key_t key;
__thread int count = 2222;
__thread unsigned long long count2 ;
static __thread int count3;
void echomsg(char* string)
{
	printf("destructor excuted in thread %x,address (%p) param=%s\n",pthread_self(),string,string);
	free(string);
}

void * child1(void *arg)
{
	int b;
	int tid=pthread_self();

	printf("I am the child1 pthread_self return %p gettid return %d\n",tid,gettid());

	char* key_content = malloc(8);
	if(key_content != NULL)
	{
		strcpy(key_content,"ACACACA");
	}
	pthread_setspecific(key,(void *)key_content);

	count=666666;
	count2=1023;
	count3=2048;
	printf("I am child1 , tid=%x ,count (%p) = %8d,count2(%p) = %6llu,count3(%p) = %6d\n",tid,&count,count,&count2,count2,&count3,count3);
	asm volatile("movl %%gs:0, %0;"
			:"=r"(b) /* output */ 
			);

	printf("I am child1 , GS address %p\n",b);

	sleep(2);
	printf("thread %x returns %x\n",tid,pthread_getspecific(key));
	sleep(50);
}

void * child2(void *arg)
{
	int b;
	int tid=pthread_self();

	printf("I am the child2 pthread_self return %p gettid return %d\n",tid,gettid());

	char* key_content = malloc(8);
	if(key_content != NULL)
	{
		strcpy(key_content,"ABCDEFG");
	}
	pthread_setspecific(key,(void *)key_content);
	count=88888888;
	count2=1024;
	count3=2047;
	printf("I am child2 , tid=%x ,count (%p) = %8d,count2(%p) = %6llu,count3(%p) = %6d\n",tid,&count,count,&count2,count2,&count3,count3);


	asm volatile("movl %%gs:0, %0;"
			:"=r"(b) /* output */ 
			);

	printf("I am child2 , GS address %p\n",b);

	sleep(1);
	printf("thread %x returns %x\n",tid,pthread_getspecific(key));
	sleep(50);
}


int main(void)
{
	int b;
	pthread_t tid1,tid2;
	printf("hello\n");


	pthread_key_create(&key,echomsg);

	asm volatile("movl %%gs:0, %0;"
			:"=r"(b) /* output */ 
			);

	printf("I am the main , GS address %x\n",b);

	pthread_create(&tid1,NULL,child1,NULL);
	pthread_create(&tid2,NULL,child2,NULL);

	printf("pthread_create tid1 = %p\n",tid1);
	printf("pthread_create tid2 = %p\n",tid2);

	sleep(60);
	pthread_key_delete(key);
	printf("main thread exit\n");
	return 0;
}
