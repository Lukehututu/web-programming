#include<stdio.h>
#include<unistd.h>

int main(int argc,char* argv[])
{
	int pid = 0;
	pid = fork();
	if(pid == 0)
	{
		printf("%s \n","I am child process!");

	}
	else
	{
		printf("Child process ID : %d\n",pid);
		sleep(30);
	}
	
	if(pid == 0)
	{
		printf("%s\n","End child process");
	
	}
	else
	{
		printf("%s\n","End parent process");
	}
	return 0;
}
