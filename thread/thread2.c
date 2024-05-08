//	thread2.c
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
void* thread_main(void* arg);


int main()
{
	pthread_t t_id;
	int thread_para = 5;
	void* thr_ret;


	if((pthread_create(&t_id,NULL,thread_main,(void*)&thread_para)) != 0)
	{
		perror("thread_create");
		return -1;
	}

	if(pthread_join(t_id,&thr_ret) != 0)
	{
		perror("pthread_join");
		return -1;
	}

	printf("msg from thread : %s",(char*)thr_ret);
	puts("end process");
	return 0;

}



void* thread_main(void* arg)
{
	int cnt = *((int*)arg);
	int i = 0;
	char* msg = (char*)malloc(sizeof(char)*50);
	strcpy(msg,"hello,i am thread !\n");

	for(i = 0;i<cnt;i++)
	{
		puts("thread running");
		sleep(1);
	}

	return (void*)msg;
}
