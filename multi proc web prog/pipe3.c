//这是pipe3.c
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define BUF_SIZE 30

int main()
{
	pid_t id;
	int fd1[2],fd2[2];
	char str1[]="Who are you ?";
	char str2[]="Thank yuo for your message !";
	char buf[BUF_SIZE];
	
	//创建管道
	pipe(fd1);
	pipe(fd2);	
	id = fork();
	if(id == 0)	//父进程（写东西）
	{
		write(fd1[1],str1,sizeof(str1));
		read(fd2[0],buf,BUF_SIZE);
		printf("Child Process output : %s id = %d\n",buf,id);		
	}
	else
	{
		read(fd1[0],buf,BUF_SIZE);
		printf("Parent process output : %s id = %d\n",buf,id);
		write(fd2[1],str2,sizeof(str2));
		//sleep(3);
	}
	return 0;

}
