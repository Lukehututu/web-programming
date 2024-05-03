// writev.c
#include<sys/uio.h>
#include<stdio.h>
#define BUF_SIZE 30

int main()
{
	struct iovec iov[2];
	char buf1[BUF_SIZE] = "1234567";
	char buf2[BUF_SIZE] = "ABCDEFG";
	int str_len;
	
	iov[0].iov_base = buf1;
	iov[0].iov_len = 4;
	iov[1].iov_base = buf2;
	iov[1].iov_len = 3;

	str_len = writev(1,iov,2);
	puts("\n");
	printf("bytes : %d\n",str_len);
	return 0;
}
