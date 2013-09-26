/*
 * =====================================================================================
 *
 *       Filename:  myls_l.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年04月10日 09时26分55秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  HeYuhang (HeYuhang), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

void show_file_info(struct stat mystat,char * fname);

int main(int argc, char *argv[])
{
	struct stat my_stat;
	struct dirent * my_dirent;
	DIR * dir;
	int node_num=0;
	char * cur_dir=".";//默认为当前目录

	if(( argc>3 || argc<2)&& argv[1]!="-l")
	{
		printf("paragram number is two and paragram is '-l'\n");
		return -1;
	}
	if(argc==3)
		cur_dir=argv[2];
//	printf("目录:%s\n",cur_dir);
	if((dir=opendir(cur_dir))==NULL)
	      printf("can not open %s\n",cur_dir);
	while((my_dirent=readdir(dir))!=NULL)
	{
		node_num+=my_dirent->d_ino;
//		printf("%s \n",my_dirent->d_name);
		if(stat(my_dirent->d_name,&my_stat)!=-1){
			show_file_info(my_stat,my_dirent->d_name);	
		}
	}
	printf("总计 %d\n",node_num);

	return 1;
}

void show_file_info(struct stat mystat,char * fname)
{
	struct tm  * mytm;
	char * month;
	char * day;
	char * hour;
	char * min;

	printf("%-5o",mystat.st_mode);
	printf("%-4d",mystat.st_nlink);
	printf("%-5d",mystat.st_uid);
	printf("%-5d",mystat.st_gid);
	printf("%-5ld",mystat.st_size);
	//时间 format
	mytm=localtime(&mystat.st_mtime);
	printf("%d-",mytm->tm_year+1990);
	if(mytm->tm_mon<10)
	      printf("0%d-",mytm->tm_mon+1);
	else printf("%d-",mytm->tm_mon+1);
	if(mytm->tm_mday<10)
	      printf("0%d ",mytm->tm_mday);
	else printf("%d ",mytm->tm_mday);
	if(mytm->tm_hour<10)
	      printf("0%d:",mytm->tm_hour);
	else printf("%d:",mytm->tm_hour);
	if(mytm->tm_min<10)
	      printf("0%d ",mytm->tm_min);
	else printf("%d ",mytm->tm_min);
	printf("%s\n",fname);
}

