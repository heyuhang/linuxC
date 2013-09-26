#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>

#define PORT 8080 //服务器端口
#define LISTENQ  10 //链接请求队列长度

struct message{
	char time[32];
	char user[32];
	char action[32];
	char content[32];
}; //消息结构体
struct MSG{
	struct message mg[10];
	int tail;
	int head;
};

/*my_recv()函数将客户端发来的json格式的buf消息匹配到Msg结构体中*/
void my_recv(char * buf,struct MSG Msg)
{
	int i,j=0,num=0;//num表示匹配判断，0匹配time，1表示匹配user....
	char str[32];

	for(i=1;i<strlen(buf);i++){
		if(buf[i]==':'){
			i++;
			while(1){
				if(buf[i]==','){
					num++;
					j=0;
					break;
				}
				if(num==0)
					Msg.mg[++Msg.tail].time[j]=buf[i];	
				else if(num==1)
					Msg.mg[++Msg.tail].user[j]=buf[i];
				else if(num==2)
					Msg.mg[++Msg.tail].action[j]=buf[i];
				else if(num==3)
					Msg.mg[++Msg.tail].content[j]=buf[i];
				i++;
										
			}		
		}	
	}	
}

void PrintMsg(struct MSG Msg)//消息处理函数，打印消息
{
	if(Msg.tail>=0){
		printf("time:%s ",Msg.mg[++Msg.head].time);	
		printf("user:%s ",Msg.mg[++Msg.head].user);
		printf("action:%s ",Msg.mg[++Msg.head].action);
		printf("content:%s ",Msg.mg[++Msg.head].content);
	}else sleep(2000);
	sleep(2000);
}

int main()
{
	int sock_fd,conn_fd;
	int optval;
	int ret;
	pid_t pid;
	socklen_t cli_len;
	struct sockaddr_in  cli_addr,serv_addr;
	char recv_buf[128];
	struct MSG Msgh[100];
	int i=0;//i为Msgh的下标

	//创建TCP套接子
	sock_fd=socket(AF_INET,SOCK_STREAM,0);
	if(sock_fd<0){
		perror("socket");
		exit(1); 
	}
	//初始化服务器端地址结构
	memset(&serv_addr,0,sizeof(struct sockaddr_in));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(PORT);
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);

	//将套接子绑定到本地端口
	if(bind(sock_fd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in))<0){
		perror("bind");
		exit(1);
	}
	//将套接子转化为监听套接字
	if(listen(sock_fd,LISTENQ)<0){
		perror("listen");
		exit(1);
	}
	cli_len=sizeof(struct sockaddr_in);
	while(1){
		conn_fd=accept(sock_fd,(struct sockaddr *)&cli_addr,&cli_len);
		if(conn_fd<0){
			perror("accept");
			exit(1);
		}
		printf("接受到一个客户端请求\n");
		Msgh[i].tail=-1;
		Msgh[i].head=-1;//初始化消息队列
		pid=fork();//创建子进程
		if(pid==0){
			while(1){
				if((ret=recv(conn_fd,recv_buf,sizeof(recv_buf),0))<0){
					perror("recv");
					exit(1);
				}
				my_recv(recv_buf,Msgh[i]);
				if(Msgh[i].tail>=10){
					
					printf("%s'频繁提问'",Msgh[i].mg[0].user);//该用户频繁请求
					sleep(5000);//等待父进程处理消息
				}
					
					
			}
			close(sock_fd);
			close(conn_fd);
			exit(0);//结束子进程
		}
		while(1){//消息处理
			PrintMsg(Msgh[i]);
		}
		close(conn_fd);
		i++;
	}
	
	return 0;
}
