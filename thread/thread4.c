//	thread4.c
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#define NUM_THREAD 100

void* thread_inc(void* arg);
void* thread_des(void* arg);
long long num = 0;	//64位整数

int main()
{
	int i = 0;
	pthread_t tid[NUM_THREAD];
	
	printf("pre num = %lld\n",num);
	for(i = 0;i < NUM_THREAD;++i)
	{
		if(i%2 == 0)
			pthread_create(&tid[i],NULL,thread_inc,NULL);
		else
			pthread_create(&tid[i],NULL,thread_des,NULL);
	}

	for(i = 0;i < NUM_THREAD;++i)
		pthread_join(tid[i],NULL);

	printf("post num = %lld\n",num);
	return 0;

}

void* thread_inc(void* arg)
{
	int i = 0;
	for(;i<5000000;++i)
		num += 1;
	return NULL;
}

void* thread_des(void* arg)
{
	int i = 0;
	for(;i<5000000;++i)
		num -= 1;
	return NULL;
}


