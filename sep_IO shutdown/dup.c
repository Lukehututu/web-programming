#include<stdio.h>
#include<unistd.h>
#define BUF_SIZE 100


int main()
{
	char buf[BUF_SIZE] = "i am luke\n";
	int fd = dup(1);	//就是stdout
	int fd2 = dup2(1,9);	
		
	printf("fd = %d , fd2 = %d \n",fd,fd2);	

	write(1,buf,sizeof(buf));
	write(fd,buf,sizeof(buf));
	write(fd2,buf,sizeof(buf));

	close(fd);
	close(fd2);

	return 0;
}
