/*
 * =====================================================================================
 *
 *       Filename:  myls.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年04月10日 08时41分35秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  HeYuhang (HeYuhang), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void do_ls(char []);

int main(int argc, char *argv[])
{
	struct dirent  * dir;
	DIR * user_dir;
	char * my_dir=".";

	if(argc>2)
	{
			printf("paragram is less two!\n");
			return -1;
	}
	if(argc!=1)
		my_dir=argv[1];
	if( (user_dir=opendir(my_dir))==NULL)
	{
		printf("dirent %s can not open!\n",argv[1]);
		return -1;
	}
	while( (dir=readdir(user_dir))!=NULL)
	{
		printf("%10s   ",dir->d_name);
	}
	printf("\n");
	  printf("\033[01;34m Hello World\033[0m\n"); 
	  printf("\033[01;30;42m hello world\033[0m\n");
	return 1;
}


