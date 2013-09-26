#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<linux/sem.h>
#include<linux/shm.h>

int main()
{
int semid,a;
struct sembuf sbuf={0,-1,IPC_NOWAIT};
union semun semopts;

  key_t key;

  
  if((key=ftok(".",'s'))==-1){//获取当前目录键值
    exit(1);
  }

  if((semid=semget(key,1,IPC_CREAT|0666))==-1){//创建含有3个信号的信号集
    exit(1);
  }
semopts.val=5;
semctl(semid,0,SETVAL,semopts);
while(1){
  semop(semid,&sbuf,1);
  a=semctl(semid,0,GETVAL,0);

//  if(a<0) printf("used \n");
  //else 
  printf("%d\n",a);
  sleep(1);
}
  semctl(semid,0,IPC_RMID);
  return 0;
  }
