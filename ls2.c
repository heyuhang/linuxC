/*
 * =====================================================================================
 *
 *       Filename:  myls_l.c
 *
 *    Description:  
 *
 *        Version:  1.3
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
#include <string.h>
#include <pwd.h>
#include <grp.h>

void show_file_info(struct stat mystat,char * fname);
void mode_format(int mode,char * str);
char * name_format(int name_num);
char * name_grp_format(int name_num);
/*
struct node
{
	char * file_name;

}linknode;*/
//char *

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
	printf("dir id:%s\n",cur_dir);
//	printf("目录:%s\n",cur_dir);
	if((dir=opendir(cur_dir))==NULL)
	{
	      printf("can not open %s\n",cur_dir);
	      return -1;
	}
	while((my_dirent=readdir(dir))!=NULL)
	{
		node_num+=my_dirent->d_ino;
//		printf("%s \n",my_dirent->d_name);
		if((lstat(my_dirent->d_name,&my_stat))!=-1)
			printf("file:%s\n",my_dirent->d_name);
		
//		show_file_info(my_stat,my_dirent->d_name);	
	}
	closedir(dir);
	printf("总计 %d\n",node_num/43044);

	return 1;
}

void show_file_info(struct stat mystat,char * fname)
{
	struct tm  * mytm;
	char * month;
	char * day;
	char * hour;
	char * min;
	char mode_buf[11];
	char * user_name;
	
	mode_format(mystat.st_mode,mode_buf);
	printf("%-10s",mode_buf);
	printf("%3d ",mystat.st_nlink);
//	printf("%-5d",mystat.st_uid);
	if((user_name=name_format(mystat.st_uid))==NULL) //UID没有对应的用户名
	      printf("%-5d",mystat.st_uid);
	else
	printf("%-9s",user_name);
	printf("%-9s",name_grp_format(mystat.st_gid));
	printf("%5ld",mystat.st_size);
	//时间 format
	mytm=localtime(&mystat.st_mtime);
	printf(" %d-",mytm->tm_year+1990);
	if(mytm->tm_mon<9)
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

void mode_format(int mode,char * str)
{
	strcpy(str,"----------");

	if(S_ISDIR(mode))
	      str[0]='d';
	if(S_ISCHR(mode))
	      str[0]='c';
	if(S_ISBLK(mode))
	      str[0]='b';
	if(S_ISFIFO(mode))
	      str[0]='p';
	if(S_ISLNK(mode))
	      str[0]='l';
	if(S_ISSOCK(mode))
	      str[0]='s';

	if(S_IRUSR & mode) str[1]='r';
	if(S_IWUSR & mode) str[2]='w';
	if(S_IXUSR & mode) str[3]='x';

	if(S_IRGRP & mode) str[4]='r';
	if(S_IWGRP & mode) str[5]='w';
	if(S_IXGRP & mode) str[6]='x';

	if(S_IROTH & mode) str[7]='r';
	if(S_IWOTH & mode) str[8]='w';
	if(S_IXOTH & mode) str[9]='x';
}

char *  name_format(int name_num)
{
	struct passwd * mypasswd;
	static char numstr[10];
	if( (mypasswd=getpwuid(name_num))==NULL)
	{
		sprintf(numstr,"%d",name_num);
		return numstr;
	}
	
	return mypasswd->pw_name;
}

char * name_grp_format(int name_num)
{
	struct group * grp;
	static char numstr[10];
	if((grp=getgrgid(name_num))==NULL)
	{
		sprintf(numstr,"%d",name_num);
		return numstr;
	}
	return grp->gr_name;
}
