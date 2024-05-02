//这是echo_mpserv.c

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
using namespace std;
void read_child(int act);

//此时将服务端封装成一个类
class tcpserver
{
public:
	int m_listenfd;		//监听的socket，-1表示未初始化	
	int m_clientfd;		//客户端脸上的socket，-1表示客户端未连接
	string m_clientip;	//客户端字符串格式的ip
	unsigned short m_port;	//服务端用于通信的端口
public:
	
	//初始化服务端
	bool initserver(const unsigned short in_port)
	{
		//1.创建服务器端socket和端口
		m_listenfd = socket(AF_INET,SOCK_STREAM,0);
		if(m_listenfd == -1)
			return false;	//创建失败直接返回false
		m_port = in_port;
		
		//2.给socket分配ip和端口
		struct sockaddr_in servaddr;		//创建该结构体来存储服务端的ip和端口和协议
		memset(&servaddr,0,sizeof(servaddr));	//初始化该结构体为0
		servaddr.sin_family = AF_INET;		//初始化为IPv4
		servaddr.sin_port = htons(m_port);	//初始化端口号
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);	//如果操作系统有多个IP，那都可以通信(此处跟服务端不同)
		
		//绑定服务器的IP和端口
		if(bind(m_listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
		{
			close(m_listenfd);
			m_listenfd = -1;
			return false;
		}
	
		//3.把服务端的socket设置为可连接(监听)的状态
		if(listen(m_listenfd,5) == -1)
		{
			close(m_listenfd);
			m_listenfd = -1;
			return false;
		}
		
		return true;
	}

	//封装accept
	bool accept()
	{
		struct sockaddr_in clientaddr;		//接收客户端的相关信息
		socklen_t addrlen = sizeof(clientaddr);	//该结构体的大小	
		//接收请求，如果没有则一直阻塞
		m_clientfd = ::accept(m_listenfd,(struct sockaddr*)&clientaddr,&addrlen);
		
		if(m_clientfd == -1)
		{
			close(m_clientfd);
			return false;
		}
		
		m_clientip = inet_ntoa(clientaddr.sin_addr);//把客户端的ip从大端序转成字符串
		return true;	//正常接收返回true
	}

	//获取客户端的ip（字符串格式）
	const string& clientip()
	{
		return m_clientip;
	}
	

	//重构发送函数
	bool send(const string& buffer)
	{
		if(m_clientfd == -1) return false;
		if( (::send(m_clientfd,buffer.data(),buffer.size(),0)) <= 0) return false;
		return true;
	}

	//重构recv函数
	bool recv(string& buffer,const size_t maxlen)
	{
		buffer.clear();
		buffer.resize(maxlen);
		int readn = ::recv(m_clientfd,&buffer[0],buffer.size(),0);
		if(readn <= 0)
		{
			buffer.clear();
			return false;
		}
		buffer.resize(readn);
		return true;
	}
	
	//注册信号
	bool initsignal(struct sigaction& act)
	{	
		//初始化
		act.sa_handler = read_child;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;		
		
		//注册信号
		sigaction(SIGCHLD,&act,0);
	}

	
	//构造和析构
	tcpserver():m_listenfd(-1),m_clientfd(-1){}



	//重构关闭两个socket的函数
	bool closelisten()
	{
		if(m_listenfd == -1) return false;
		
		::close(m_listenfd);
		m_listenfd = -1;
		return true;
	}

	
	bool closeclient()
	{
		if(m_clientfd == -1) return false;
		::close(m_clientfd);
		m_clientfd = -1;
		return true;
	}

		
	~tcpserver(){closelisten();closeclient();}
};


void read_child(int sig)
{
	int status;
	pid_t id;
	while((id = waitpid(-1,&status,WNOHANG)) > 0)
	{
		if(WIFEXITED(status))
		{	
			cout<<"remove process id :" << id <<endl;
			cout<<"child send :" << WEXITSTATUS(status)<<endl;
		}
	}
}





int main(int argc, char *argv[])
{

	tcpserver tcpserver;
	//初始化服务端并检查是否成功
	if(tcpserver.initserver(atoi(argv[1])) == false)
	{
		perror("initserver");
		return -1;
	}


    	if (argc != 2)
    	{
        	printf("Usage : %s <port>\n", argv[0]);
        	exit(1);
	}
	//创建变量
  	string buffer;
	struct sigaction act;
	tcpserver.initsignal(act);		
	pid_t id;
	int sock;
	
	while(true)
	{	
		//先accept，如果接收到请求，那就创建子进程
		if(tcpserver.accept() == false)
		{
			continue;
		}
		//此时监听到了消息,fork进程
		id = fork();
		if(id > 0)	//父进程		
			continue;
		else if(id == 0)	//子进程
		{
			while(tcpserver.recv(buffer,1024) != false)		
			{
				cout<<"接收："<<buffer<<endl;
				if(tcpserver.send(buffer) == false)
				{
					perror("send");
					return 0;
				}
			}

			return 0;
		}

	}

	
	return 0;
}

