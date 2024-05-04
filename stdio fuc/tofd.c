#include<stdio.h>
#include<fcntl.h>


int main()
{
	FILE* fp;
	fp = fopen("data.txt","w");
	if(fp == NULL)
	{
		fputs("file open error!",stdout);
		return -1;
	}
	
	int fd = fileno(fp);
	
	write(fd,"hello luke！\n",sizeof("hello luke! \n"));
	close(fd);
	return 0;
}

