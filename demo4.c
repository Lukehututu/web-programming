
//这是demo4.c

#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>
using namespace std;

void error_handling(char *message);

//封装客户端成类
class tcpclient
{
//暂且将所有类都放在public中
public:
	int m_clientfd;		//客户端的socket，-1表示未连接或者连接已断开，>=0表示有效的socket
	string m_ip;		//客户端的ip/域名
	unsigned short m_port;	//客户端的端口号
	
	//封装connect函数，向服务端发送连接请求，成功返回true，失败返回false
	bool connect(const string& in_ip,const unsigned short in_port)
	{
		if(m_clientfd != -1)return false;	//如果socket已连接，那就直接返回失败	
		//初始化成员变量
		m_ip = in_ip;
		m_port = in_port;
		
		//第一步，创建客户端的socket
		m_clientfd = socket(AF_INET,SOCK_STREAM,0);
		if(m_clientfd == -1)
			return false;
	
		//第二步，向服务器发起连接请求
		struct sockaddr_in servaddr;		//用于存放通信协议，IP地址，端口的结构体
		memset(&servaddr,0,sizeof(servaddr));	//初始化该变量
		servaddr.sin_family = AF_INET;		//1.proto为IPv4
		servaddr.sin_port = htons(m_port);	//2.传入端口号
		
		struct hostent* h;			//用于获取大端序IP地址的结构体的指针
		if( (h = gethostbyname(m_ip.c_str())) == nullptr)
		{
			close(m_clientfd);
			m_clientfd = -1;			//如果失败了就重新置为-1，对应该变量的声明
			return false;
		}
		memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);	//将获取的ip地址（大端序）放入变量中
		//向服务端发起请求
		if(::connect(m_clientfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
		{
			close(m_clientfd);
			m_clientfd = -1;
			return false;
		}
		
		//如果都执行成功
		return true;
	}	
	
	
	//封装send向服务端发送报文，成功返回true，失败返回false
	bool send(const string& buffer)		//此处不用const char#
	{

		if(m_clientfd == -1)return false;//如果是未连接则返回flase
		
		//如果发送失败，返回false
		if(::send(m_clientfd,buffer.data(),buffer.size(),0)<=0) return false;
		
		//如果成功返回true
		return true;
	}

	//接受服务端的报文，成功返回true，失败返回false
	//其中buffer->存放接收到的buffer的内容，maxlen-本次接受报文的最大长度
	bool recv(string &buffer ,const size_t maxlen)		//如果直接操作string对象的内存，必须保证1.不能越界，2.操作后手动设置数据大小
	{
		buffer.clear();			//清空容器
		buffer.resize(maxlen);		//重新设置容器大小为maxlen
		int readn = ::recv(m_clientfd,&buffer[0],buffer.size(),0);	//直接操作buffer的内存
		//其中recv对象的返回值 -1 失败  0 socket连接已断开 >0则成功收到了数据
		if(readn <= 0)
		{
			buffer.clear(); //在推出前把buffer清空，否则大小还是maxlen
			return false;	//判断是否接收到了数据,没接收到就直接返回false
		}

		buffer.resize(readn);		//将buffer调整为适合的大小
		return true;
	}	


	
	//先将cocket初始化为-1
	tcpclient():m_clientfd(-1){}		//构造函数


	~tcpclient(){}		//析构函数
	
};



int main(int argc, char *argv[])
{
    
  	//创建对象
	tcpclient tcpclient;
	//向服务端发送连接请求
	if(tcpclient.connect(argv[1],atoi(argv[2])) == false)
	{
		perror("connect");
		return -1;
	}
	cout<<"连接成功"<<endl;
	
	//第三步，与服务端通信，客户发送一个请求报文后等待服务端的应答，收到回复后，再发送下一个报文
	string buffer;
	for(int i = 0; i<10 ;i++)
	{
		buffer = "这是第" + to_string(i+1) + "个产品,编号" + to_string(i+1) + ".";	//生成请求报文内容
		//向服务端发送请求报文
		if(tcpclient.send(buffer)<=0)
		{
			perror("send");
			break;
		}
		cout << "发送: " << buffer << endl;
	
		//接收服务端的回应报文，如果服务端没有发送回应报文，recv()函数将阻塞等待
		if( tcpclient.recv(buffer,1024) == false)
		{
			perror("recv");
			break;
		}	
		cout << "接收："<< buffer << endl;
		sleep(1);
	}
	
	//第四步，关闭socket释放资源
    	close(tcpclient.m_clientfd);
  	return 0;
}

/*
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
*/
