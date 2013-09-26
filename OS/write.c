#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<linux/sem.h>
#include<linux/shm.h>

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
  struct sembuf buf={0,1,IPC_NOWAIT};

  if(index<0){
    return -1;
  }
  buf.sem_num=index;
  if(semop(semid,&buf,1)==-1){
    return -1;
  }
  return 0;
}

int get_sem(void)
{
  key_t key;
  int semid;
 
  
  if((key=ftok(".",'s'))==-1){//获取当前目录键值
    exit(1);
  }

  if((semid=semget(key,1,IPC_CREAT|0666))==-1){//创建含有3个信号的信号集
    exit(1);
  }

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
  char * add;

  semid=get_sem();
  shmid=create_shm();
  if((add=shmat(shmid,(char *)0,0))==(char *)-1)
     exit(1);

  while(1){
    sem_p(semid,1);
    sem_p(semid,2);
    printf("read:%s\n",add);

//    printf("%d ",semctl(semid,0,GETVAL,0));
//    printf("%d\n",semctl(semid,1,GETVAL,1));
    sleep(2);
    sem_v(semid,2);
    sem_v(semid,0);

    sleep(2);
  }
  return 0;
}

