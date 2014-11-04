/*
$gcc -Wall -lpthread hack_thread_sched.c -o hack_thread_sched
$./hack_thread_sched 
policy=SCHED_OTHER
Show current configuration of priority
max_priority=0
min_priority=0
show SCHED_FIFO of priority
max_priority=99
min_priority=1
show SCHED_RR of priority
max_priority=99
min_priority=1
show priority of current thread
priority=0
Set thread policy
set SCHED_FIFO policy
policy=SCHED_FIFO
priority=0
set SCHED_RR policy
policy=SCHED_RRRestore current policy
priority=0

*/
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <assert.h>

static int get_thread_policy(pthread_attr_t *attr)
{
	int policy;
	int rs = pthread_attr_getschedpolicy(attr,&policy);
	assert(rs==0);

	switch(policy)
	{
		case SCHED_FIFO:
			printf("policy=SCHED_FIFO\n");
			break;

		case SCHED_RR:
			printf("policy=SCHED_RR\n");
			break;

		case SCHED_OTHER:
			printf("policy=SCHED_OTHER\n");
			break;

		default:
			printf("policy=UNKNOWN\n");
			break;
	}
	return policy;
}

static void show_thread_priority(pthread_attr_t *attr,int policy)
{
	int priority = sched_get_priority_max(policy);
	assert(priority != -1);
	printf("max_priority=%d\n",priority);

	priority= sched_get_priority_min(policy);
	assert(priority != -1);
	printf("min_priority=%d\n",priority);
}

static int get_thread_priority(pthread_attr_t *attr)
{
	struct sched_param param;
	int rs = pthread_attr_getschedparam(attr,&param);
	assert(rs == 0);

	printf("priority=%d\n",param.__sched_priority);
	return param.__sched_priority;
}

static void set_thread_policy(pthread_attr_t *attr,int policy)
{
	int rs = pthread_attr_setschedpolicy(attr,policy);
	assert(rs==0);
}

int main(void)
{
	pthread_attr_t attr;
	int rs;

	rs = pthread_attr_init(&attr);
	assert(rs==0);

	int policy = get_thread_policy(&attr);

	printf("Show current configuration of priority\n");
	get_thread_policy(&attr);
	show_thread_priority(&attr,policy);

	printf("show SCHED_FIFO of priority\n");
	show_thread_priority(&attr,SCHED_FIFO);

	printf("show SCHED_RR of priority\n");
	show_thread_priority(&attr,SCHED_RR);

	printf("show priority of current thread\n");
	get_thread_priority(&attr);

	printf("Set thread policy\n");

	printf("set SCHED_FIFO policy\n");
	set_thread_policy(&attr,SCHED_FIFO);
	get_thread_policy(&attr);
	get_thread_priority(&attr);

	printf("set SCHED_RR policy\n");
	set_thread_policy(&attr,SCHED_RR);
	get_thread_policy(&attr);

	printf("Restore current policy\n");
	set_thread_policy(&attr,policy);
	get_thread_priority(&attr);

	rs = pthread_attr_destroy(&attr);
	assert(rs==0);

	return 0;
}
