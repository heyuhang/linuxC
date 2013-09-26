/*
 * =====================================================================================
 *
 *       Filename:  mywho.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年04月09日 09时34分00秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  HeYuhang (HeYuhang), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <utmp.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
int main(int argc, char *argv[])
{
	struct utmp user_record;
	int utmp;

	if((utmp=open(UTMP_FILE,O_RDONLY))==-1)
	{
		return -1;
	}
	printf("username  hostname  device  time\n");
	while(read(utmp,&user_record,sizeof(user_record)))
	{
//		printf("username  hostname  device  time\n");
		printf("%s  %s  %s %s\n",user_record.ut_user,user_record.ut_host,user_record.ut_line,ctime(&user_record.ut_time));
	}
	close(utmp);
	return 1;
}


