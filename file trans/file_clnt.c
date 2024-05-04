//	file_clnt.c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUF_SIZE 1024

int main(int argc,char* argv[])
{
	int clnt_sock;
	struct sockaddr_in clntadd;
	char buf[BUF_SIZE];

	clnt_sock = socket(AF_INET,SOCK_STREAM,0);

	memset(&clntadd,0,sizeof(clntadd));
	clntadd.sin_family = AF_INET;
	clntadd.sin_port = htons(atoi(argv[2]));
	clntadd.sin_addr.s_addr = inet_addr(argv[1]);

	if(connect(clnt_sock,(struct sockaddr*)&clntadd,sizeof(clntadd)) == -1)
	{	
		perror("connect");
		return -1;
	}

	FILE*fp = fopen("recv.dat","wb");
	int readlen;
	while((readlen = read(clnt_sock,buf,BUF_SIZE)) != 0)
		fwrite((void*)buf,1,readlen,fp);
	
	puts("接收完成");
	write(clnt_sock,"Thank you!",15);

	fclose(fp);
	close(clnt_sock);
	return 0;
}
