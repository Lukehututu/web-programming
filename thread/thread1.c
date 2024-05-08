//	thread1.c
#include<stdio.h>
#include<pthread.h>
void* thread_main(void* arg);


int main()
{
	pthread_t t_id;
	int thread_para = 5;

	if((pthread_create(&t_id,NULL,thread_main,(void*)&thread_para)) != 0)
	{
		perror("thread_create");
		return -1;
	}

	
	sleep(10);
	puts("end process");
	return 0;

}



void* thread_main(void* arg)
{
	int cnt = *((int*)arg);
	int i = 0;
	for(i = 0;i<cnt;i++)
	{
		puts("thread running");
		sleep(1);
	}
}
