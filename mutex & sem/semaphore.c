//	semaphore.c
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

static sem_t sem1;
static sem_t sem2;
static int sum = 0;
void* thread_in(void* arg);
void* thread_add(void* arg);


int main()
{
	int i = 0;
	pthread_t tid_a,tid_b;

	//创建sem
	sem_init(&sem1,0,0);
	sem_init(&sem2,0,1);

	pthread_create(&tid_a,NULL,thread_in,NULL);
	pthread_create(&tid_b,NULL,thread_add,NULL);

	pthread_join(tid_a,NULL);
	pthread_join(tid_b,NULL);

	//当结束后，销毁sem
	sem_destroy(&sem1);	
	sem_destroy(&sem2);	


	return 0;

}

void* thread_in(void* arg)
{
	int i = 0;
	for(;i<5;++i)
	{
		fputs("Input num :",stdout);
		sem_wait(&sem2);
		scanf("%d",&sum);		
		sem_post(&sem1);
	}
	return NULL;
}

void* thread_add(void* arg)
{
	int i = 0;
	int res = 0;
	for(;i<5;++i)
	{
		sem_wait(&sem1);
		res += sum;
		sem_post(&sem2);
	}

	printf("Result = %d\n",res);
	return NULL;
}


