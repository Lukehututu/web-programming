#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define BUF_SIZE 30

int main()
{
	pid_t id;
	int fd[2];
	char str[]="Who are you ?";
	char buf[BUF_SIZE];
	
	//创建管道
	pipe(fd);	//此时就已经拿到IO描述符
	id = fork();
	if(id == 0)	//父进程（写东西）
	{
		write(fd[1],str,sizeof(str));		
	}
	else
	{
		read(fd[0],buf,BUF_SIZE);
		puts(buf);
	}
	return 0;

}
