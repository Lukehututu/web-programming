//这里是get_buf.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

int main(int argc,char* argv[])
{	
	//创建变量
	int sock;
	int snd_buf,rcv_buf,state;
	socklen_t len;
	//初始化变量
	sock = socket(AF_INET,SOCK_STREAM,0);
	len = sizeof(snd_buf);
	state = getsockopt(sock,SOL_SOCKET,SO_SNDBUF,(void*)&snd_buf,&len);
	if(state)
	{
		perror("getsockopt");
		return -1;
	}
	
	len = sizeof(rcv_buf);
	state = getsockopt(sock,SOL_SOCKET,SO_SNDBUF,(void*)&rcv_buf,&len);
	if(state)
	{
		perror("getsockopt");
		return -1;
	}
	
	printf("SNDBUF : %d \n",snd_buf);
	printf("RCVBUF : %d \n",rcv_buf);
	return 0;
}
