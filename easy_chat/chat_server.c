//	chat_server.c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<semaphore.h>
#define BUF_SIZE 100
#define MAX_CLNT 256
void* handle_clnt(void* arg);
void send_msg(char* buf,int len);
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;
int clnt_cnt = 0;

int main(int argc,char* argv[])
{
	int serv_sock,clnt_sock;
	socklen_t addrlen;
	struct sockaddr_in servadd,clntadd;
	pthread_t t_id;	


	serv_sock = socket(AF_INET,SOCK_STREAM,0);
	//注册一个互斥量
	pthread_mutex_init(&mutx,NULL);


	memset(&servadd,0,sizeof(servadd));
	servadd.sin_family = AF_INET;
	servadd.sin_family = htons(atoi(argv[2]));
	servadd.sin_family = htonl(INADDR_ANY);

	if(bind(serv_sock,(struct sockaddr*)&servadd,sizeof(servadd)) == -1)
	{
		perror("bind");
		return -1;
	}

	if(listen(serv_sock,5) == -1)
	{
		perror("listen");
		return -1;
	}
	
	while(1)
	{
		addrlen = sizeof(clntadd);
		clnt_sock = accept(serv_sock,(struct sockaddr*)&clntadd,&addrlen);
		
		pthread_mutex_lock(&mutx);
		clnt_socks[clnt_cnt++] = clnt_sock;		
		pthread_mutex_unlock(&mutx);

		pthread_create(&t_id,NULL,handle_clnt,(void*)&clnt_sock);
		pthread_detach(t_id);
		printf("Connected Client IP : %s\n",inet_ntoa(clntadd.sin_addr));
	}
	close(serv_sock);
	return 0;
}


void* handle_clnt(void* arg)
{
	int clnt_sock = *((int*)arg);
	int str_len,i;
	char buf[BUF_SIZE];

	while((str_len = read(clnt_sock,buf,sizeof(buf))) != 0 )
		send_msg(buf,str_len);

	//此时说明读取结束，该关闭客户端的连接了
	pthread_mutex_lock(&mutx);
	for(i = 0;i < clnt_cnt;++i)
	{
		//找到该sock，然后将后面的sock往前移
		if(clnt_sock == clnt_socks[i])
		{
			while(i++ < clnt_sock-1)
				clnt_socks[i] = clnt_socks[i+1];
		}	
		break;		
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);
	close(clnt_sock);
	return NULL;
}

//发送信息给所有客户端
void send_msg(char* buf,int len)
{
	int i;
	pthread_mutex_lock(&mutx);
	for(i = 0;i < clnt_cnt ;i++)
	{
		write(clnt_socks[i],buf,len);
	}
	pthread_mutex_unlock(&mutx);
}


