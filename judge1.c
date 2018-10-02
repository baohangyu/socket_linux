#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
//小车云平台，通过按键实现
#define portnum 1649 //端口号不小于1600

int main (void)
{
	int err = 1;
	int sockfd, new_fd,binfd,listfd;
	struct sockaddr_in server_addr;//服务器地址
	struct sockaddr_in clint_addr; //客户机地址
	char buffer[128];
	int nbite;
	int sin_size;
	char judge[1024];
	int client,others;
//1.1 创建套接字
	if((sockfd=socket( AF_INET, SOCK_STREAM, 0)) <= 0)//如果创建套接字失败
	{
		printf("create socket error!\n");
		exit(1);
	}
	printf("socket = %d\n",sockfd);
//1.2设置套接字端口重复利用
	int on = 1;
	if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))==0)
		printf("端口重复利用成功\n");
	else
		printf("端口重复利用失败\n");
//2.1 设置要绑定的地址，对server_addr初始化
	bzero(&server_addr,sizeof(struct sockaddr_in));//全部成员清0
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons( portnum);//转换成网络字节序，定义端口。
	server_addr.sin_addr.s_addr = htonl( INADDR_ANY);//转换成网络字节序
//2.2 绑定地址
	binfd=bind( sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr) );
	if (binfd == -1)
		printf("bind = %d，绑定地址失败\n",binfd);
	else
		printf("绑定地址成功\n");
//3. 监听端口
	listfd=listen(sockfd, 5);
	if(binfd <= -1)
	{
		printf("监听端口:%d失败\n",portnum);
	}
	else
	{	
		printf("IP地址为：134.175.0.71\n");
		printf("监听端口:%d成功\n",portnum);
	}
	char sendbuf[1024];
	char recvbuf[1024];
	char together[1024];//client发送给云平台的数据存储到该字符串中，等待客户端接收。
	
	while(1)
	{
		while(1)
		{
			sin_size = sizeof(struct sockaddr);
			printf("等待连接...\n");
			new_fd=accept(sockfd,(struct sockaddr *)(&clint_addr),&sin_size);	
			printf("server get connection from %s\n",(inet_ntoa(clint_addr.sin_addr)));
			memset(together,0,sizeof(together));
			nbite=recv( new_fd, together, sizeof(together), 0);
			buffer[nbite] = '\0';
			printf("已接收: %s\n",buffer );
			if(strcmp(buffer,"_CLIENT_")==0)
			{
				printf("智能家居已经连接上...\n");
				client=new_fd;
				char ok[] = "ok\n"; 
				send(new_fd,ok,sizeof(ok),0);	
				err=1;
			}
			else
			{	
				printf("用户已连接云平台...\n");
				others=new_fd;
				err=2;
			}
			memset(recvbuf,0,sizeof(recvbuf));	
			break;
		}
		if(err==1)
		{	
			printf("client已连接\n");
			nbite = recv(new_fd, recvbuf,sizeof(recvbuf), 0);	
			recvbuf[nbite] = '\0';
			strcpy(together,recvbuf);
			memset(recvbuf,0,sizeof(recvbuf));
		}
		if(err==2);
		{
			printf("用户已连接\n");
			char success[] = "恭喜你已连接上云平台\n";
			send(others,success,sizeof(success),0);
			//sendbuf[] = together[];
			strcpy(sendbuf,together);
			printf("%s",together);
			send(new_fd,sendbuf,sizeof(sendbuf),0);
			printf("已发送：%s",sendbuf);
			memset(sendbuf,0,sizeof(sendbuf));
		}
		printf("已经循环到了底部\n");
		close(new_fd);
		close(client);
		close(others);
	}
	close(sockfd);	
}
		
/*		
		
	
//while(1)
	{
		sin_size = sizeof(struct sockaddr);
                new_fd=accept(sockfd,(struct sockaddr *)(&clint_addr),&sin_size);
		printf("server get connection from %s\n",(inet_ntoa(clint_addr.sin_addr)) );
		if (nbite = recv( new_fd, recvbuf, 128, 0)!=0)	
		{
			recvbuf[nbite] = '\0';
                        printf("已接收：%s",recvbuf);
		}
		memset(recvbuf,0,sizeof(recvbuf));
	}		
	close(new_fd);
	while(1)   
	{
//4. 等待连接
		printf("62");
		sin_size = sizeof(struct sockaddr);
		printf("63");
		new_fd=accept(sockfd,(struct sockaddr *)(&clint_addr),&sin_size);
		printf("server get connection from %s\n",(inet_ntoa(clint_addr.sin_addr)) );//打印出连接服务器的客户机地址
//5.1 接收数据
		printf("67");
		if (nbite = recv( new_fd, recvbuf, 128, 0)==0)//返回接收到的数据长度
			printf("服务器未接收到数据...");
		else
		{
			recvbuf[nbite] = '\0';
			printf("已接收：%s",recvbuf);
		}
		memset(recvbuf,0,sizeof(recvbuf));
//		sleep(10);
		while(fgets(sendbuf,sizeof(sendbuf),stdin)!=NULL)
		{
			//memset(recvbuf,0,sizeof(recvbuf));
			nbite=recv( new_fd, buffer, 128, 0);
			buffer[nbite] = '\0';
			printf("已接收: %s\n",buffer );
			//char ok[] = "陈忠涛大傻帽\n";
//5.2发送程序，发送特定字符串到客户端
			//int ret = read(new_fd,recvbuf,sizeof(recvbuf));
			fputs(recvbuf,stdout);
			send(new_fd,sendbuf,sizeof(sendbuf),0);
			printf("已发送：%s",sendbuf);
			memset(sendbuf,0,sizeof(sendbuf));
			break;
		}
//6. 结束连接
		close(new_fd);
	}
	close(sockfd);
	return 0;
}*/
