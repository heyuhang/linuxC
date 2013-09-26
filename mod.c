#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#define LEN 100
main()
{
	int fd;
	int len;
	char str[LEN];
	char filename[512];
	printf( "Input file name : \n" );
	scanf("%s",filename);	
	fd= open(filename,O_RDONLY);
	if( fd == -1 )
	{
		printf("open failed !\n");
		return ;	
	}
	len = read(fd,str,LEN);  
	str[len] = '\0';
	printf("读取到的内容是: %s\n",str);
	sleep(20);
	len = read(fd,str,LEN);
	str[len] = '\0';
	printf("再次读取到的内容是: %s\n",str);
	close(fd);
}
