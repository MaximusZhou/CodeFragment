#include<stdio.h>
#include<string.h>/*Get `memset`*/

struct s
{
	int member;
};

typedef struct s s_array[10];  /*define a array type*/

int main(void)
{
	int i;
	s_array array_var;

	memset(array_var,0,sizeof(s_array));

	for(i = 0 ; i < 10; i ++)
	{
		printf("%d\n",array_var[i].member);
	}

	return 0;
}
