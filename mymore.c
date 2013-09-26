#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>

#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *f,long fsize);
int see_more(FILE *cmd,int per);

int main(int argc,char ** argv)
{
	FILE * fd;
	struct stat buf;
	fd=fopen(argv[1],"r");
	if(argc==1)
		do_more(stdin,0);
	else if((fd=fopen(argv[1],"r"))!=NULL)
	{
		stat(argv[1],&buf);
		//printf("fsize=%ld\n",(long)buf.st_size);
		do_more(fd,(long)buf.st_size);
		fclose(fd);
	}

	return 1;
}

void do_more(FILE *f,long fsize)
{
	char buffline[LINELEN];
	struct stat buf;
	int  curline=0,replay;
	FILE *tty;
	int per;
	while(fgets(buffline,LINELEN,f))
	{
		tty=fopen("/dev/tty","r");
		if(curline==PAGELEN){
			//printf("%ld\n",ftell(f));
			per=(int)((float)ftell(f)/(float)fsize*100);
			
			replay=see_more(tty,per);
			if(replay==0)
				break;
			curline-=replay;
		}
		
		if(fputs(buffline,stdout)==EOF)
			exit(1);
		curline++;
		
	}
}

int see_more(FILE *cmd,int per)
{
	char c;
	printf("\033[7m --more--?%d%\033[m",per);
	if( (c=getc(cmd))!=EOF)
	{
		if(c=='q')
			return 0;
		if(c=='y')
			return PAGELEN;
		if(c=='\n')
			return 1;
			
	}
	return 0;
}
