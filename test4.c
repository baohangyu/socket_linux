
#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#define portnum 1562/随便定义一个端口


int main()
{
	int err = 1;
	int sockfd, new_fd,binfd,listfd;
	struct sockaddr_in server_addr;//服务器地址
	struct sockaddr_in clint_addr; //客户机地址
	char buffer[128];
	int nbite;
	int sin_size;

//1.1 创建套接字
	if((sockfd=socket( AF_INET, SOCK_STREAM, 0)) == -1)//如果创建套接字失败
	{
		printf("create socket error!\n");
	}
	printf("socket = %d\n",sockfd);
//1.2设置套接字端口重复利用
	int on = 1;
	if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0)
	printf ("端口重复利用失败");
//2.1 设置要绑定的地址，对server_addr初始化
	bzero(&server_addr,sizeof(struct sockaddr_in));//全部成员清0
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portnum);//转换成网络字节序，定义端口。
	server_addr.sin_addr.s_addr = htonl( INADDR_ANY);//转换成网络字节序
//2.2 绑定地址
	binfd=bind( sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr) );
	if (binfd == -1)
		printf("bind = %d，绑定地址失败\n",binfd);
	else
		printf("绑定地址成功\n");
//3. 监听端口
	listfd=listen(sockfd, 5);
	if(binfd == -1)
	{
		printf("监听端口:%d失败",portnum);
	}
	else
	{
		printf("监听端口:%d成功，等待发送数据...\n",portnum);
	}
	while (1)
	{
//4. 等待连接
		sin_size = sizeof(struct sockaddr);
		new_fd=accept(sockfd,(struct sockaddr *)(&clint_addr),&sin_size);
		printf("server get connection from %s\n",(inet_ntoa(clint_addr.sin_addr)) );//打印出连接服务器的客户机地址
//5.1 接收数据
		if (nbite = recv( new_fd, buffer, 128, 0)<=0)//返回接收到的数据长度
			printf("服务器未接收到数据...%d",nbite);
		else
		{	
			buffer[nbite] = '\0';
			printf("server received: %s\n",buffer );
		}
		char ok[] = "cai cai wo shi shei";
//5.2发送程序，发送特定字符串到客户端
		send(new_fd,ok,sizeof(ok),0);
		printf("已发送：“%s”\n",ok);
		sleep(5);/*sleep*/
	close(new_fd);
	}
	close(sockfd);
	return 0;
}
