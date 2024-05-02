#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>


int main()
{
	int state;
	pid_t pid = fork();
	
	if(pid == 0)
		return 3;
	else
	{	
		printf("Child process pid : %d \n",pid);
		pid = fork();
		if(pid == 0)
		{
			sleep(30);
			exit(7);
		}
		else
		{
			printf("Child pid : %d\n",pid);
			wait(&state);
			if(WIFEXITED(state))
				printf("Child send one : %d \n",WEXITSTATUS(state));
	
			wait(&state);
			if(WIFEXITED(state))
				printf("Child send one : %d \n",WEXITSTATUS(state));
			sleep(30);
		}
	}
	return 0;




}
