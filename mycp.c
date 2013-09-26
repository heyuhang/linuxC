/*
 * =====================================================================================
 *
 *       Filename:  mycp.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年04月09日 10时13分54秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  HeYuhang (HeYuhang), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 1024
#define CMODE   0664  //复制的目标文件创建权限mode
int main(int argc, char *argv[])
{
	int fd_s,fd_d,buf_len;
	char buf[BUFSIZE];

	if(argc<3)
	{
		printf("mycp [source_file] [target_file]\n");
		return -1;
	}

	if((fd_s=open(argv[1],O_RDONLY))==-1)
	{
		return -1;
	}
	if(strcmp(argv[2]))
	if((fd_d=creat(argv[2],CMODE))==-1)
	{
		return -1;
	}
	while((buf_len=read(fd_s,&buf,BUFSIZE)))
	{
		write(fd_d,buf,buf_len);
	}
	return 1;
}


