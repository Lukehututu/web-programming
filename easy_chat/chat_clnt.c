//	chat_clnt.c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<semaphore.h>
#define BUF_SIZE 100
#define NAME_SIZE 20

char buf[BUF_SIZE];
char name[NAME_SIZE] ="[DEFAULT]";
void* recv_msg(void* arg);
void* send_msg(void* arg);


int main(int argc,char* argv[])
{
	int clnt_sock;
	socklen_t addrlen;
	struct sockaddr_in clntadd;
	pthread_t recv_id,send_id;	


	clnt_sock = socket(AF_INET,SOCK_STREAM,0);
	
	sprintf(name,"[%s]",argv[3]);

	memset(&clntadd,0,sizeof(clntadd));
	clntadd.sin_family = AF_INET;
	clntadd.sin_port = htons(atoi(argv[2]));
	clntadd.sin_addr.s_addr = inet_addr(argv[1]);

	if(connect(clnt_sock,(struct sockaddr*)&clntadd,sizeof(clntadd)) == -1)
	{
		perror("connect");
		return -1;
	}
	
	void* thread_ret;	

	//创建IO线程并引导销毁
	pthread_create(&recv_id,NULL,recv_msg,(void*)&clnt_sock);
	pthread_create(&send_id,NULL,send_msg,(void*)&clnt_sock);
	pthread_join(recv_id,thread_ret);
	pthread_join(send_id,thread_ret);
	close(clnt_sock);	
	return 0;
}


void* send_msg(void* arg)
{
	int sock = *((int*)arg);
	char name_msg[NAME_SIZE+BUF_SIZE];
	while(1)
	{
		fgets(buf,BUF_SIZE,stdin);
		if( !strcmp(buf,"q\n") )
		{
			close(sock);
			exit(0);
		}
		sprintf(name_msg,"%s %s",name,buf);
		write(sock,name_msg,strlen(name_msg));
	}
	return NULL;
}


void* recv_msg(void* arg)
{
	int sock = *((int*)arg);
	char name_msg[NAME_SIZE + BUF_SIZE];
	int str_len;

	while(1)
	{
		str_len = read(sock,name_msg,NAME_SIZE + BUF_SIZE - 1);
		if(str_len == -1)
		{
			close(sock);
			return (void*)-1;
		}
		name_msg[str_len] = 0;
		fputs(name_msg,stdout);
	}
	return NULL;
}

