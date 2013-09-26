#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>


//{"time":1323333"user":"xxx","action":"answer","content":"abcde"}


int main(int argc,char ** argv)//命令行获取地址和端口
{
	int conn_fd;
	int serv_port;
	struct sockaddr_in  serv_addr;
	char	send_buf[]="{'time':1323333,'user':'xxx','action':'answer','content':'abcde'}";//初始化消息

	if(argc<3){
		printf("输入地址和端口!");
		exit(1);	
	}

	//初始化服务端地址结构
	memset(&serv_addr,0,sizeof(struct sockaddr_in));
	serv_addr.sin_family=AF_INET;
	if(inet_aton(argv[1],&serv_addr.sin_addr)==0){
		perror("inet_aton");
		exit(1);
	}	
	serv_port=atoi(argv[2]);
	//创建TCP套接子
	conn_fd=socket(AF_INET,SOCK_STREAM,0);
	if(conn_fd<0){
		printf("socket创建失败\n");
		exit(1);
	}
	//发送链接请求
	if(connect(conn_fd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr))<0){
		printf("链接失败\n");
		printf("%s--%d\n",argv[1],serv_port);
		exit(1);
	}
	while(1){//不断请求
		if(send(conn_fd,send_buf,strlen(send_buf),0)<0){
			printf("发送失败\n");
			exit(1);	
		}
	}
	
	return 0;
}

