#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<linux/sem.h>
#include<linux/shm.h>

#define EMPTY 10
#define FULL 0
#define MUTEX 1
#define SEMNUM 3

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

  if((semid=semget(key,SEMNUM,IPC_CREAT|0666))==-1){//创建含有3个信号的信号集
    exit(1);
  }

  //设置信号量的值
  semopts1.val=EMPTY;
  if(semctl(semid,0,SETVAL,semopts1)==-1)
    return -1;
  semopts2.val=FULL;
  if(semctl(semid,1,SETVAL,semopts2)==-1)
    return -1;
  semopts3.val=MUTEX;
  if(semctl(semid,2,SETVAL,semopts3)==-1)
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

  if((shmid=shmget(key,EMPTY,IPC_CREAT|0666))==-1){//创建10个共享内存
    exit(1);
  }
  return shmid;
}

int main()
{
  int semid,shmid;
  int  * add;

  semid=create_sem();
  shmid=create_shm();
  if((add=shmat(shmid,NULL,0))==-1)
     exit(1);
  
  while(1){
    sem_p(semid,0);
    sem_p(semid,2);
  
    * add=1+rand()%10;
    printf("produce:%d  ",*add);
    add++;
    
    printf("%d ",semctl(semid,0,GETVAL,0));
    printf("%d\n",semctl(semid,1,GETVAL,1));
    sleep(6);
  
    sem_v(semid,2);
    sem_v(semid,1);

    sleep(6);
  }
  return 0;
}

