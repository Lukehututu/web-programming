#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>


int main()
{
	int state;
	pid_t pid = fork();
	
	if(pid == 0)
	{		
		sleep(15);	
		return 3;
	}
	else
	{	
		printf("Child process pid : %d \n",pid);
		while(!waitpid(-1,&state,WNOHANG))
		{
			sleep(3);
			puts("sleep 3sec");
		}
		if(WIFEXITED(state))
		{
			printf("Child send %d \n",WEXITSTATUS(state));
		}

	}	
	return 0;




}
