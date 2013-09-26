#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string.h>
#include<linux/sem.h>
#include<linux/shm.h>

#define EMPTY 10
#define FULL 0
#define MUTEX 1

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
  int semid;
  union semun semopts1,semopts2,semopts3;
 
  
  if((key=ftok(".",'s'))==-1){//获取当前目录键值
    exit(1);
  }

  if((semid=semget(key,1,IPC_CREAT|0666))==-1){//创建含有3个信号的信号集
    exit(1);
  }

  //设置信号量的值
  semopts1.val=MUTEX;
  if(semctl(semid,0,SETVAL,semopts1)==-1)
    return -1;

  return semid;
}

int create_shm()
{
  key_t key;
  int shmid;

  if((key=ftok(".",'s'))==-1){//获取当前目录键值
    exit(1);
  }

  if((shmid=shmget(key,10,IPC_CREAT|0666))==-1){//创建10个共享内存
    exit(1);
  }
  return shmid;
}

int main()
{
  int semid,shmid;
  char * add,write[10];

  semid=create_sem();
  shmid=create_shm();
  if((add=shmat(shmid,(char *)0,0))==(char *)-1)
     exit(1);
  
  while(1){
    sem_p(semid,0);
    sem_p(semid,2);
    printf("write: ");
    scanf("%s",&write);
    write[sizeof(write)+1]='\0';
    strcpy(add,write);
//    printf("%d ",semctl(semid,0,GETVAL,0));
//    printf("%d\n",semctl(semid,1,GETVAL,1));
    sleep(2);
  
    sem_v(semid,2);
    sem_v(semid,1);

    sleep(2);
  }
  return 0;
}

