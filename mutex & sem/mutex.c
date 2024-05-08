//	mutex.c
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#define NUM_THREAD 100

pthread_mutex_t mutex;
void* thread_inc(void* arg);
void* thread_des(void* arg);
long long num = 0;	//64位整数

int main()
{
	int i = 0;
	pthread_t tid[NUM_THREAD];

	//创建mutex
	pthread_mutex_init(&mutex,NULL);

	
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

	//当结束后，销毁mutex
	pthread_mutex_destroy(&mutex);

	return 0;

}

void* thread_inc(void* arg)
{
	int i = 0;
	pthread_mutex_lock(&mutex);
	for(;i<5000000;++i)
	{
		num += 1;
	}
	pthread_mutex_unlock(&mutex);
	return NULL;
}

void* thread_des(void* arg)
{
	int i = 0;
	pthread_mutex_lock(&mutex);
	for(;i<5000000;++i)
	{
		num -= 1;
	}
	pthread_mutex_unlock(&mutex);
	return NULL;
}


