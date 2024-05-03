//echo_selectserv.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/socket.h>
using namespace std;
#define BUF_SIZE 30



int main(int argc,char* argv[])
{
	//1.create socket
	int m_listenfd,clientfd;
	m_listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(m_listenfd == -1)
	{
		perror("socket");
		return -1;
	}
	//2.init socket
	struct sockaddr_in serv_addr,clnt_addr;
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[1]));
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//3.config source to socket
	if(bind(m_listenfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1)
	{	
		perror("bind");
		return -1;	
	}
	//4.start up listen
	if(listen(m_listenfd,5) == -1)
	{		
		perror("listen");	
		return -1;	
	}
	//准备变量
	char buf[BUF_SIZE];
	struct timeval timeout;
	int fd_max,strlen,fd_num,i;
	fd_set reads,cp_reads;
	socklen_t add_sz;

	//初始化reads
	FD_ZERO(&reads);	
	FD_SET(m_listenfd,&reads);
	fd_max = m_listenfd;	
	

	while(1)
	{	
		//最外层，监听listenfd，并建立连接
		cp_reads = reads;		//重置reads，和timeout
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;
		//监视
		if((fd_num = select(fd_max + 1,&cp_reads,0,0,&timeout))== -1)
			break;
		else if(fd_num == 0)	//超时	
		{
			cout<<"Time out!"<<endl;
			continue;
		}
		//检测到有要读的信息	
		//循环检查是哪个fd有信息要读
		for(i = 0;i < fd_max+1;i++ )
		{
			if(FD_ISSET(i,&cp_reads))
			{	
				if(i == m_listenfd)		//说明有新的连接请求
				{
					//创建新的套接字，并更新fd_max
					add_sz = sizeof(clnt_addr);
					clientfd = accept(m_listenfd,(struct sockaddr*)&clnt_addr,&add_sz);
					//如果分配的新的客户端fd比fd_max大那就更新max
					if(fd_max < clientfd)
						fd_max = clientfd;
					//给新的clientfd注册进fd_set里
					FD_SET(clientfd,&reads);
					cout<<"Connect Client : "<<clientfd<<endl;
				
				}	
				else 				//说明是其他fd的信息，说明要读
				{
					strlen = read(i,buf,BUF_SIZE);
					if(strlen == 0)//说明读到文件尾，那就关闭连接
					{	
						FD_CLR(i,&reads);
						close(i);
						cout<<"Close Client :"<<i<<endl;
					}
					else	
					{
						cout<<"Client msg : " << buf << endl;
						write(i,buf,strlen);	//回声
					}
				}	
			}	
		}
	}
	close(m_listenfd);
	return 0;
}


