//echo_serv.c
//改造成标准IO的形式
#include <cstdio>
#include <fcntl.h>
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
#define BUF_SIZE 100

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

	FILE* readfp,*writefp;
	
	//5.accept client
	char buf[BUF_SIZE];
	while(true)
	{	
		struct sockaddr_in clnt_sock;
		socklen_t clnt_adr_size = sizeof(clnt_sock);
		int clientfd = accept(m_listenfd,(struct sockaddr*)&clnt_sock,&clnt_adr_size);
		if(clientfd == 0) continue;
		else if(clientfd == -1) cout<<"系统资源不足"<<endl;
		

		//转换成文件指针
		readfp = fdopen(clientfd,"r");
		writefp = fdopen(clientfd,"w");
	
		//already accept then start tansx
		string m_clientip = inet_ntoa(clnt_sock.sin_addr);
		cout<<"连接成功,IP为："<< m_clientip <<endl;
		while(!feof(readfp))
		{
			fgets(buf,BUF_SIZE,readfp);
			cout<<"接受"<<buf;
			fputs(buf,writefp);
			fflush(writefp);
		}
		fclose(readfp);	
		fclose(writefp);
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

