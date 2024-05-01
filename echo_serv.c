//echo_serv.c

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
using namespace std;
void error_handling(const string message,int fd);


int main(int argc,char* argv[])
{
	//1.create socket
	int m_listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(m_listenfd == -1)
		error_handling("socket",m_listenfd);
	//2.init socket
	struct sockaddr_in serv_addr;
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[1]));
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//3.config source to socket
	if(bind(m_listenfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1)
		error_handling("bind",m_listenfd);
	
	//4.start up listen
	if(listen(m_listenfd,5) == -1)
		error_handling("listen",m_listenfd);
	
	//5.accept client
	string buffer;
	while(true)
	{	
		struct sockaddr_in clnt_sock;
		socklen_t clnt_adr_size = sizeof(clnt_sock);
		int clientfd = accept(m_listenfd,(struct sockaddr*)&clnt_sock,&clnt_adr_size);
		if(clientfd == 0) continue;
		else if(clientfd == -1) cout<<"系统资源不足"<<endl;
		
		//already accept then start tansx
		string m_clientip = inet_ntoa(clnt_sock.sin_addr);
		cout<<"连接成功,IP为："<< m_clientip <<endl;
		while(true)
		{
			buffer.resize(1024);
			int readn = recv(clientfd,&buffer[0],buffer.size(),0);
			if(readn <= 0)
				break;
			buffer.resize(readn);
			cout<<"接收："<<buffer<<endl;
			buffer.clear();
		}
		close(clientfd);	
		cerr<<"接收失败"<<endl;
	}
	close(m_listenfd);
	return 0;
}



void error_handling(const string message,int fd)
{
	close(fd);
	fd = -1;
	exit(1);
}

