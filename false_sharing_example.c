/*
$gcc --std=c99 false_sharing_example.c -o false_sharing_example -fopenmp
*/
#include<stdio.h>   
#include<omp.h>   
#include<string.h>   
#include<time.h>   
#include<stdlib.h>   

#define NMAX 4096 * 10   
#define NUM_CORE 2   

int g_iBuff[NMAX];   


int main()   
{   
	int alignPos = 0;   
	for (int i = 0; i < 65; ++i)   
	{     
		if (((long)&g_iBuff[i]) % 64 == 0)   
		{     
			alignPos = i;   
			break;   
		}     
	}     
	printf("alignPos = %d, sizof(int) = %d\n", alignPos, sizeof(int));
	for (int pos = 1; pos <= 4096; pos <<= 1)   
	{     
		memset(g_iBuff, 0 , sizeof(g_iBuff));   
		time_t beg = time(0);   
#pragma omp parallel for num_threads(NUM_CORE)   
		for (int i = 0; i < NUM_CORE; ++i)   
		{     
			int tp = i * pos + alignPos;   
			printf("thread-id:%d, tp = %d\n", omp_get_thread_num(), tp);
			for (int j = 0; j < 999999999; ++j)   
			{     
				g_iBuff[tp]++;   
			}     
		}     
		time_t end = time(0);   

		printf("false shareing: step = %d time = %ds\n", pos, end - beg);
	}     

	return 0;   
}  
