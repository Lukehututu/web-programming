//	file_server.c

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 1024
void error_handler(char* error,int sock);


int main(int argc,char* argv[])
{
	int serv_sock,clnt_sock;
	struct sockaddr_in servadd,clntadd;
	FILE* fp;
	

	serv_sock = socket(AF_INET,SOCK_STREAM,0);

	memset(&servadd,0,sizeof(servadd));
	servadd.sin_family = AF_INET;
	servadd.sin_port = htons(atoi(argv[1]));
	servadd.sin_addr.s_addr = htonl(INADDR_ANY);


	if(bind(serv_sock,(struct sockaddr*)&servadd,sizeof(servadd)) == -1)
		error_handler("bind",serv_sock);
	
	if(listen(serv_sock,5) == -1)
		error_handler("listen",serv_sock);

	socklen_t addlen = sizeof(servadd);
	if((clnt_sock = accept(serv_sock,(struct sockaddr*)&clntadd,&addlen)) == -1)
		error_handler("accept",clnt_sock);
	
	fp = fopen("dup.c","rb");
	int str_len;
	char buf[BUF_SIZE];	

	puts("现在开始发送文件");

	while(1)
	{
		str_len = fread((void*)buf,1,BUF_SIZE,fp);
		if(str_len < BUF_SIZE)	//此时已经读到文件最后的部分了，因此发送完信息就退出
		{
			write(clnt_sock,buf,str_len);
			break;
		}
		write(clnt_sock,buf,str_len);
	}

	puts("文件发送结束");
	//使用shutdown半关闭，同时发送EOF给client
	shutdown(clnt_sock,SHUT_WR);

	read(clnt_sock,buf,BUF_SIZE);
	printf("Client msg : %s\n",buf);

	fclose(fp);
	close(serv_sock);
	close(clnt_sock);
	return 0;
}


void error_handler(char* error,int sock)
{
	perror(error);
	close(sock);
}
