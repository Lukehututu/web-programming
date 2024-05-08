//	thread3.c
#include<stdio.h>
#include<pthread.h>
void* thread_main(void* arg);
int sum = 0;


int main()
{
	pthread_t tid1,tid2;
	int argv1[] = {1,5};
	int argv2[] = {6,10};

	//创建线程
	pthread_create(&tid1,NULL,thread_main,(void*)argv1);
	pthread_create(&tid2,NULL,thread_main,(void*)argv2);
	
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);

	printf("sum = %d\n",sum);
	return 0;
}


void* thread_main(void* arg)
{
	int start = ((int*)arg)[0];
	int end = ((int*)arg)[1];

	while(start<=end)
	{
		sum+=start;
		start++;
	}


}
