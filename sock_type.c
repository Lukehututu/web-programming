//这是 sock_type.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>


//介绍getsockopt的使用方法
int main(int argc,char* argv[])
{
	//创建
	int tcp_sock;
	int sock_type;
	socklen_t optlen;
	int state;

	//初始化变量包括获取套接字
	optlen = sizeof(sock_type);
	tcp_sock = socket(AF_INET,SOCK_STREAM,0);
	printf("SOCK_STREAM : %d \n",SOCK_STREAM);
	
	//读取套接字可选项
	state = getsockopt(tcp_sock,SOL_SOCKET,SO_TYPE,(void*)&sock_type,&optlen);
	//0成功，-1失败，因此直接state
	if(state)	//失败处理
	{
		perror("state");
		return -1;
	}
	printf("sock_type : %d \n",sock_type);
	
	return 0;
}
