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
#include <linux/limits.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <malloc.h>
#ifdef TIOCGWINSZ
#include <sys/ioctl.h>
#endif
#include <asm/ioctls.h>

struct node
{
	char * file_name;
	struct stat time;
	int index;
	struct node * next;
	struct node * prev;
};

struct winsize {
        unsigned short ws_row;
        unsigned short ws_col;
        unsigned short ws_xpixel;
        unsigned short ws_ypixel;
};

static int wsize=0;
static int maxlen=0;
static int len=0;
 
void show_file_info(struct stat mystat,char * fname);
void mode_format(int mode,char * str);
char * name_format(int name_num);
char * name_grp_format(int name_num);
void init_file_node(struct node ** head);
struct node * sort_file(struct node *  head);
void entry_file_node(struct node * head,char * fname,struct stat utime,int index);
void print_file(struct node * head);
char * path_slipt(char * fname);

void init_file_node(struct node ** head)
{
	(*head)=(struct node *)malloc(sizeof(struct node));
	(*head)->next=NULL;
	(*head)->prev=NULL;
}

void entry_file_node(struct node * head,char * fname,struct stat utime,int index)
{
	struct node * new_node,* p;
	p=head;
	while(p->next){
		p=p->next;
	}
	new_node=(struct node *)malloc(sizeof(struct node));
	new_node->file_name=fname;
	new_node->time=utime;
	new_node->index=index;
	p->next=new_node;
	new_node->prev=p;
	new_node->next=NULL;
}

struct node * sort_file(struct node * head)
{
	struct node * shead,* new,*p,*q,*s;
	char a,b;

	p=head->next->next;
	init_file_node(&shead);
	new=(struct node *)malloc(sizeof(struct node));
	new->file_name=head->next->file_name;
	new->time=head->next->time;
	shead->next=new;
	q=shead->next;
	q->prev=shead;
	free(head);
	free(head->next);
	while(p!=NULL)
	{
		a=p->file_name[0];
		b=q->file_name[0];
		if(p->file_name[0]=='.')
		      a=p->file_name[1];
		if(q->file_name[0]=='.')
		      b=q->file_name[1];
		if(a>=b){
			new=(struct node *)malloc(sizeof(struct node));
			new->file_name=p->file_name;
			new->time=p->time;
			new->index=p->index;
			q->next=new;
			new->prev=q;
			q=new;
			q->next=NULL;				
		}
		else{
			s=q;
			while(a<b && q!=NULL){
				q=q->prev;
				if(q!=shead)
				{
					b=q->file_name[0];
					if(q->file_name[0]=='.')
			      			b=q->file_name[1];
				}else break;

			}
			new=(struct node *)malloc(sizeof(struct node));
			new->file_name=p->file_name;
			new->time=p->time;
			new->index=p->index;
			new->next=q->next;
			q->next->prev=new;
			q->next=new;
			new->prev=q;
			q=s;
		}
		s=p;
		p=p->next;
		free(s);
	}
	return shead;
}

void print_file(struct node * head)
{
	struct node * p;

	p=head->next;
	while(p){
		show_file_info(p->time,p->file_name);
		p=p->next;
	}
}

char * path_slipt(char * fname)
{
	int i=0;
	char * path;
	path=(char *)malloc(sizeof(char)*(strlen(fname)+1));
	strcpy(path,fname);
	i=strlen(fname)-1;
	while(i>=0)
	{
		if(path[i]=='/')
		{
			path[i+1]='\0';
			break;	
		}
		i--;
	}
	return path;
}

int strcmps(char * a,char b)
{
	int i=0;
	if(a[0]!='-')
	      return -1;
	while(i<strlen(a)){
	      if(a[i]==b)
		    return 0;
	      i++;
	}
	return -1;
}

void show_a_s(char * path)
{
	struct stat buf;
	if(lstat(path,&buf)==-1)
	      printf("myls:无法访问 %s：没有那个文件或目录\n",path);
	else printf("%s\n",path);
}
void show_l_s(char * path)
{
	struct stat buf;
	if(lstat(path,&buf)!=-1)
	      show_file_info(buf,path);
	else printf("myls:无法访问 %s:没有那个文件或目录\n",path);
}

void show_a_l(struct node * head,int rows,int cols)
{
	struct node * p,* q;
	int i=0,j,k=1,s=0;
	
	p=q=head->next;
	while(k<=rows){
		for(j=0;j<cols;j++)
		{
			i=0;
			printf("%-s",p->file_name);
			for(s=strlen(p->file_name);s<=maxlen;s++)
			      printf(" ");
			for(;i<rows && p!=NULL;i++)
			      	p=p->next;
			if(p==NULL)
				break;
		}
		printf("\n");
		k++;
		p=q->next;
		q=q->next;
	}
}

void show_l_l(struct node *  head)
{
	struct node * p;

	p=head->next;
	while(p){
		show_file_info(p->time,p->file_name);
		p=p->next;
	}
}

void show_l(struct node * head,int rows)
{
	struct node * p;

	p=head->next;
	while(p)
	{
		if(p->file_name[0]!='.')
			printf("%s\n",p->file_name);
		p=p->next;
	}
}

struct node * read_dir(DIR * dir,struct node * head,char * cur_dir,int * rows,int * cols)
{
	struct dirent * my_dirent;
	struct node * fnode;
	struct stat my_stat;
	int name_max=0,size=0;
	char fpath[PATH_MAX+1];

	strcat(cur_dir,"/");
	while((my_dirent=readdir(dir))!=NULL)
	{
		if(my_dirent->d_reclen>name_max)
		      name_max=my_dirent->d_reclen;
		strcpy(fpath,cur_dir);
		strcat(fpath,my_dirent->d_name);
		if((lstat(fpath,&my_stat))!=-1){
			//show_file_info(my_stat,my_dirent->d_name);
			entry_file_node(head,my_dirent->d_name,my_stat,0);
			size+=(strlen(my_dirent->d_name)+3);
			if(strlen(my_dirent->d_name)>maxlen)
				maxlen=strlen(my_dirent->d_name);
			len++;
		}
	} 	
	fnode=sort_file(head);
	printf("size:%d\n",size);
	if(wsize==1){
		(*cols)=135/maxlen;
		(*rows)=len/(*cols);
		if(len%(*cols)>0)
			(*rows)+=1;
	}
	else{
		(*cols)=80/maxlen;
		(*rows)=len/(*cols);
		if(len%(*cols)>0)
			(*rows)+=1;
	}
	printf("len: %d  rows:%d  cols:%d\n",len,(*rows),(*cols));
	return fnode;
}

int is_dir(char * dir)
{
	struct stat jstat;
	if(lstat(dir,&jstat)!=-1)
	      if(S_ISDIR(jstat.st_mode))
		    return 1;
	return -1;
}

void get_dir_link(char * path)
{
	DIR * dir;
	struct dirent * my_dirent;
	struct node * fnode;
	struct stat my_dstat;
	char fpath[PATH_MAX+1];
	int rows=0,cols=0;

	if((dir=opendir(path))!=NULL)
	{
		while((my_dirent=readdir(dir))!=NULL )
		{
			if(strcmp(my_dirent->d_name,"..")==0)
			      continue;
			strcpy(fpath,path);
			strcat(fpath,my_dirent->d_name);
			strcat(fpath,"/\0");
			if(is_dir(fpath)==1)
			{
//				printf("目录：%s\n",fpath);
				if((dir=opendir(fpath))!=NULL)
				{
					init_file_node(&fnode);			
					fnode=read_dir(dir,fnode,fpath,&rows,&cols);
					show_l_l(fnode);
				}
				get_dir_link(fpath);				
			}
		} 
	}
	closedir(dir);
}

void show_r_l(char * cur_dir)
{
	get_dir_link(cur_dir);
}

void del_dir_node(struct node * head)
{
	struct node * p,* q;
	p=head->next;

	while(p)
	{
		if(p->file_name[0]=='.')
		{
			q=p;
			p->prev->next=p->next;
			p->next->prev=p->prev;
			free(q);
		}
		p=p->next;
	}
}

int main(int argc, char *argv[])
{

	struct node * fnode;
	DIR * dir;
	char * cur_dir,* file_name;//默认为当前目录
	int i;
	int mode_a=0,mode_l=0,mode_u=0,mode_r=0;//参数号
        struct winsize size;
	int rows=0,cols=0;
	//获取窗口大小
        if(ioctl(STDIN_FILENO, TIOCGWINSZ, (char*)&size) < 0)
                return -1;
        printf("%d rows, %d columns\n", size.ws_row, size.ws_col);
	if(size.ws_col>80)
		wsize=1;
	cur_dir=(char *)malloc(sizeof(char)*PATH_MAX);
	strcpy(cur_dir,".\0");
	for(i=1;i<argc;i++)
	{
		if(strcmp(argv[i],"-a")==0 || strcmps(argv[i],'a')==0)
		{
			mode_a=8;	
		}
		if(strcmp(argv[i],"-l")==0 || strcmps(argv[i],'l')==0)
		{
			mode_l=4;
		}
		if(strcmp(argv[i],"-u")==0 || strcmps(argv[i],'u')==0)
		{
			mode_u=2;
		}
		if(strcmp(argv[i],"-r")==0 || strcmps(argv[i],'r')==0)
		{
			mode_r=1;
		}
		if(argv[i][0]!='-')
			cur_dir=argv[i];
	}
	printf("目录:%s\n",cur_dir);
	init_file_node(&fnode);
	if((dir=opendir(cur_dir))==NULL)
	{
		strcat(cur_dir,"/");
		if(mode_l==4)
		      show_l_s(cur_dir);
		else show_a_s(cur_dir);

		return 1;
	}
	fnode=read_dir(dir,fnode,cur_dir,&rows,&cols);
	if(mode_a==0 && mode_l==0 && mode_u==0 && mode_r==0){
		show_l(fnode,rows);
		return 1;
	}else{
		if(mode_a!=8)
		{
			del_dir_node(fnode);
		}
		if(mode_r==1)
		{
			show_r_l(cur_dir);
			return 1;
		}
		if(mode_a!=8 && mode_l!=4)
		{
			show_l(fnode,rows);
			return 1;
		}else if(mode_a==8 && mode_l!=4)
		{
			show_a_l(fnode,rows,cols);
			return 1;
		}
		else 
		{
			show_l_l(fnode);
			return 1;
		}
	}
	closedir(dir);

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
	mytm=gmtime(&mystat.st_mtime);
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
