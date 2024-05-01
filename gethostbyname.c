//这是gethostbyname.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>


int main(int argc , char* argv[])
{
	struct hostent* host;
	if(argc != 2)
	{
		printf("Usage : %S <addr> \n",argv[0]);
		exit(1);
	}
	//获取host结构体信息
	host = gethostbyname(argv[1]);
	if(!host)
	{
		perror("gethostbyname");
		exit(1);
	}
	//开始打印
	printf("Offical name : %s \n",host->h_name);
	int i = 0;
	for(i = 0 ; host->h_aliases[i];i++)
		printf("Aliases %d : %s \n",i+1,host->h_aliases[i]);
	printf("Address types: %s \n",
		host->h_addrtype == AF_INET ?" AF_INET" : "AF_INET6");
	for(i = 0 ; host->h_addr_list[i];i++)
	{
		printf("IP %d : %s \n",
			i+1,inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
	}	
	return 0;

}
