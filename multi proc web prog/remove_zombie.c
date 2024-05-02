#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>


//handler来处理子进程
void read_child(int sig)
{
	int statu = 0;
	pid_t id = waitpid(-1,&statu,WNOHANG);
	if(WIFEXITED(statu))
	{
		printf("remove process : %d\n",id);
		printf("Child send : %d\n",WEXITSTATUS(statu));
	}

}


int main()
{
	struct sigaction act;
	pid_t id = 0;
	//初始化act
	act.sa_handler = read_child;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	//注册信号
	sigaction(SIGCHLD,&act,0);
	
	//开始开子进程
	id = fork();
	if(id == 0)	//子进程代码块
	{
		puts("Hi,I am child process !");
		sleep(10);
		exit(5);
	}
	else	//父进程代码块
	{
		printf("Child pid : %d\n",id);
		id = fork();
		if(id == 0)	//子进程
		{	
			puts("Hi,I am child process !");
			sleep(15);
			return 15;
		}
		else		//父进程
		{
			int i = 0;
			printf("Child pid : %d\n",id);
			for(i = 0;i < 5;i++)
			{
				puts("wait...");
				sleep(5);
			}
		}
	}
	return 0;

}
