//select.c
#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/select.h>
#define BUF_SIZE 30


int main()
{
	//创建好变量
	fd_set reads,temps;
	int result,strlen;
	char buf[BUF_SIZE];
	struct timeval timeout;
	
	//初始化变量
	FD_ZERO(&reads);
	FD_SET(0,&reads);	//0是标准输入
	
	/*
 	timeout.tv_sec = 5;
	timeout.tv_usec = 5000;
	*/
	
	while(1)
	{
		temps = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		result = select(1,&temps,0,0,&timeout);
		if(result == -1)
		{
			puts("select() error!");
			break;
		}
		else if(result == 0)
		{
			puts("time out!");
			break;
		}
		else
		{
			if(FD_ISSET(0,&temps))
			{
				strlen = read(0,buf,BUF_SIZE);
				buf[strlen] = 0;
				printf("message from console : %s",buf);				

			}		
		}
	}

	return 0;
}
