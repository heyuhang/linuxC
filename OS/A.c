#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<linux/sem.h>
#include<linux/shm.h>

#define KUAI 5

int sem_p(int semid,int index)
{
  struct sembuf buf={0,-1,IPC_NOWAIT};

  if(index<0){
    return -1;
  }
  buf.sem_num=index;
  if(semop(semid,&buf,1)==-1){
    return -1;
  }
  return 0;
}

int sem_v(int semid,int index)
{
  struct sembuf buf={0,+1,IPC_NOWAIT};

  if(index<0){
    return -1;
  }
  buf.sem_num=index;
  if(semop(semid,&buf,1)==-1){
    return -1;
  }
  return 0;
}

int create_sem(void)
{
  key_t key;
  int semid,i;
  union semun semopts[KUAI];
 
  
  if((key=ftok(".",'z'))==-1){//获取当前目录键值
    exit(1);
  }

  if((semid=semget(key,KUAI,IPC_CREAT|0666))==-1){//创建含有3个信号的信号集
    exit(1);
  }

  //设置信号量的值
  for(i=0;i<KUAI;i++){
        semopts[i].val=1;
  	if(semctl(semid,i,SETVAL,semopts[i])==-1)
    		return -1;
  }
  return semid;
}

void action(int i,int semid)
{
  while(1){
    if(semctl(semid,i,GETVAL,i)==1 && semctl(semid,(i+1)%KUAI,GETVAL,(i+1)%KUAI)==1){
       sem_p(semid,i);
       sem_p(semid,(i+1)%KUAI);
  
       printf("%d:eating...\n",i);
       sleep(3);
  
       sem_v(semid,i);
       sem_v(semid,(i+1)%KUAI);

       printf("%d：thinking...\n",i);
       sleep(3);
    }
  }
}

int main()
{
  int semid,i;
  pid_t pid[KUAI],child_pid;

  semid=create_sem();

  
  for(i=1;i<=KUAI;i++){
	child_pid=fork();
        switch(child_pid){
		case -1:exit(1);
	
		case 0:
		       action(i,semid);
                       exit(1);
		default:pid[i-1]=child_pid;break;		
	}
        sleep(4);
  }
  wait();
  return 0;
}

