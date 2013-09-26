/*
 * =====================================================================================
 *
 *       Filename:  utmplib.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年04月09日 12时20分54秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  HeYuhang (HeYuhang), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <fcntl.h>
#include <utmp.h>
#include <stdio.h>
#include <sys/types.h>

#define NRECS  16
#define NULLUT ((struct utmp *)NULL)
#define UTSIZE (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE]
static int num_recs
static int cur_rec
static int fd_utmp=-1

utmp _open(char * filename)
{
	fd_utmp=open(filename,O_RDONLY);
	cur_rec=num_recs=0;
	return fd_utmp;
}

struct utmp * utmp_next()
{
	struct utmp * recp;
	if(fd_utmp==-1)
	{
		return NULLUT;
	}
	if(cur_rec==num_recs && utmp_reload()==0)
	{
		return NULLUT;
	}
	RECP=(struct utmp *)&utmpbuf[cur_]
}

int main(int argc, char *argv[])
{


	return EXIT_SUCCESS;
}


