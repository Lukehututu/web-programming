//echo_EPLTserv.c

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
#include <sys/epoll.h>
using namespace std;
#define BUF_SIZE 2
#define EPOLL_SIZE 50


int main(int argc,char* argv[])
{
	int m_listenfd,clientfd;
	m_listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(m_listenfd == -1)
	{
		perror("socket");
		return -1;
	}

	struct epoll_event* ep_events;
	struct epoll_event event;
	int epfd,event_cnt;
		

	struct sockaddr_in serv_addr,clnt_addr;
	int i;

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[1]));
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(bind(m_listenfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1)
	{	
		perror("bind");
		return -1;	
	}
	

	if(listen(m_listenfd,5) == -1)
	{		
		perror("listen");	
		return -1;	
	}
	
	//创建epoll例程
	epfd = epoll_create(EPOLL_SIZE);
	size_t ep_sz = sizeof(struct epoll_event);
	ep_events = (epoll_event*)malloc( ep_sz * EPOLL_SIZE);
	
	//注册事件和fd
	event.events = EPOLLIN;
	event.data.fd = m_listenfd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,m_listenfd,&event);


	//准备变量
	char buf[BUF_SIZE];
	socklen_t add_sz;
	int strlen;

	while(1)
	{	
		//开始监听
		event_cnt = epoll_wait(epfd,ep_events,EPOLL_SIZE,-1);
		if(event_cnt == -1)//出错
		{
			perror("epoll_wait");
			break;
		}
		cout<<"return epoll_wait"<<endl;	
		//检测到有要读的信息	
		//循环去处理触发事件的fd
		for(i = 0;i < event_cnt ;i++ )
		{
			if(ep_events[i].data.fd == m_listenfd)
			{	
				//创建新的套接字，并更新fd_max
				add_sz = sizeof(clnt_addr);
				clientfd = accept(m_listenfd,(struct sockaddr*)&clnt_addr,&add_sz);
				event.events = EPOLLIN;
				event.data.fd = clientfd;
				epoll_ctl(epfd,EPOLL_CTL_ADD,clientfd,&event);
				cout<<"Connect Client : "<<clientfd<<endl;
			
			}	
			else 				//说明是其他fd的信息，说明要读
			{
				strlen = read(ep_events[i].data.fd,buf,BUF_SIZE);
				if(strlen == 0)//说明读到文件尾，那就关闭连接
				{	
					epoll_ctl(epfd,EPOLL_CTL_DEL,ep_events[i].data.fd,NULL);
					close(ep_events[i].data.fd);
					cout<<"Close Client :"<<ep_events[i].data.fd<<endl;
				}
				else	
				{
					cout<<"Client msg : " << buf << endl;
					write(ep_events[i].data.fd,buf,strlen);	//回声
				}
			}	
		}	
	}

	close(m_listenfd);
	close(epfd);
	return 0;
}


