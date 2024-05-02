//这是pipe2.c
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define BUF_SIZE 30

int main()
{
	pid_t id;
	int fd[2];
	char str1[]="Who are you ?";
	char str2[]="Thank yuo for your message !";
	char buf[BUF_SIZE];
	
	//创建管道
	pipe(fd);	//此时就已经拿到IO描述符
	id = fork();
	if(id == 0)	//父进程（写东西）
	{
		write(fd[1],str1,sizeof(str1));
		sleep(2);
		read(fd[0],buf,BUF_SIZE);
		printf("Child Process output : %s\n",buf);		
	}
	else
	{
		read(fd[0],buf,BUF_SIZE);
		printf("Parent process output : %s\n",buf);
		write(fd[1],str2,sizeof(str2));
		sleep(3);
	}
	return 0;

}
