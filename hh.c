#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<stdlib.h>
#include<string.h>

int match(char * filename)
{
	int i=0;
	while(1){
		if(filename[i]!='\0')
			i++;
		else break;	
	}	
	if( (filename[i-1]=='c' || filename[i-1]=='C') && filename[i-2]=='.' )
		return 1;
	return 0;
}

int find(char * content)
{
	int i=0;
	while(1){
		if(content[i]=='/' && content[i+1]=='/')
			return 1;
		else if(content[i]=='/' && content[i+1]=='*')
			return 2;
		else if(content[i]=='\0')
			break;
		i++;
	}
	
	return 0;
}

void link(char * x,char *y)
{
	int i=0,j=0;

	while(++i){
		if(x[i]=='\0')
		 break;
	}
	while(1){
		if(y[j]=='\0')
		{
			x[i]=y[j];
			break;
		}
		x[i]=y[j];
		j++;
		i++;
	}
}

int main(int argc,char ** argv)
{
	DIR * dir;
	struct dirent * ptr;
	FILE * fd;
	char * line=NULL,path[]={'\0'};
	size_t len=0;
	ssize_t read;
	int sum=0,tag=0;

	if(argc<2){
		printf("输入目录名!");
		exit(1);	
	}
	if((dir=opendir(argv[1]))==NULL){//打开目录
		return -1;
	}

	while((ptr=readdir(dir))!=NULL){
		if(match(ptr->d_name)){//获取文件名
			printf("argv=%s\n",argv[1]);
			strcat(path,argv[1]);
			strcat(path,"/");
			strcat(path,ptr->d_name);
			printf("%s\n",path);
			printf("%s\n",ptr->d_name);
			
			if((fd=fopen(path ,"rb"))!=NULL){
				path[0]='\0';
				while((read=getline(&line,&len,fd))!=-1){
					
					tag=find(line);
					if(tag==1){//tag==1时为一个单行注释
						printf("%s-%d:%s\n",ptr->d_name,sum+1,line);
						sum++;
						tag=0;
					}	
					else if(tag==2){//tag==2时为一个多行注释
						printf("%s-%d:%s\n",ptr->d_name,sum+1,line);
						sum++;
						tag=0;
					}
				}
				fclose(fd);
							
			}else printf("不能打开！\n");	
		}
				
	}
	printf("sum=%d\n",sum);
	closedir(dir);

	return 0;
}
