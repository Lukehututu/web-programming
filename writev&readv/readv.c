#include<stdio.h>
#include<sys/uio.h>
#define BUF_SIZE 30


int main()
{
	struct iovec iov[2];
	char buf1[BUF_SIZE];
	char buf2[BUF_SIZE];
	int str_len;

	iov[0].iov_base = buf1;
	iov[0].iov_len = 5;
	iov[1].iov_base = buf2;
	iov[1].iov_len = 13;


	str_len = readv(0,iov,2);
	printf("readv bytes : %d\n ",str_len);

	puts(buf1);
	puts(buf2);
	return 0;

}
