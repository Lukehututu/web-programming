#include<stdio.h>
#define BUF_SIZE 3

int main()
{
	FILE* fd1,*fd2;
	char buf[BUF_SIZE];

	fd1 = fopen("data.txt","r");
	fd2 = fopen("cpy2.txt","w");

	while( fgets(buf,BUF_SIZE,fd1) != NULL)
		fputs(buf,fd2);

	fclose(fd1);
	fclose(fd2);
	return 0;
}
