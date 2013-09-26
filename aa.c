#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define M 10 // 缓冲数目

int in = 0;   // 生产者放置产品的位置
int out = 0; // 消费者取产品的位置

int buff[M] = {0}; // 缓冲初始化为0， 开始时没有产品

sem_t empty_sem; // 同步信号量， 当满了时阻止生产者放产品
sem_t full_sem;   // 同步信号量， 当没产品时阻止消费者消费
pthread_mutex_t mutex; // 互斥信号量， 一次只有一个线程访问缓冲

int product_id = 0;   //生产者id
int prochase_id = 0; //消费者id

/* 
打印缓冲情况 
*/
void print()
{
int i;
for(i = 0; i < M; i++)
   printf("%d ", buff[i]);
printf("\n");
}

/* 
生产者
方法 
*/ 
void *product()
{
int id = ++product_id;

while(1)
{
   // 用sleep的数量可以调节生产和消费的速度，便于观察
   sleep(1);
  
   sem_wait(&empty_sem);
   pthread_mutex_lock(&mutex);
  
   in = in % M;
   printf("product%d in %d. like: \t", id, in);
  
   buff[in] = 1;  
   print();  
   ++in;
  
   pthread_mutex_unlock(&mutex);
   sem_post(&full_sem);  
}
}

/*www
消费
者方法 
www*/
void *prochase()
{
int id = ++prochase_id;
while(1)
{
   // 用sleep的数量可以调节生产和消费的速度，便于观察
   sleep(1);
  
   sem_wait(&full_sem);
   pthread_mutex_lock(&mutex);
  
   out = out % M;
   printf("prochase%d in %d. like: \t", id, out);
  
   buff[out] = 0;
   print();
   ++out;
  
   pthread_mutex_unlock(&mutex);
   sem_post(&empty_sem);
}
}

int main()
{
pthread_t id1;
pthread_t id2;

// 初始化同步信号量
int ini1 = sem_init(&empty_sem, 0, M); 
int ini2 = sem_init(&full_sem, 0, 0);  
if(ini1 && ini2 != 0)
{
   printf("sem init failed \n");
   exit(1);
} 
//初始化互斥信号量 
int ini3 = pthread_mutex_init(&mutex, NULL);
if(ini3 != 0)
{
   printf("mutex init failed \n");
   exit(1);
} 
// 创建生产者线程

   if((pthread_create(&id1, NULL, product, NULL))!=0)
   {
    printf("product creation failed \n");
    exit(1);
   }

//创建消费者线程
   if((pthread_create(&id2, NULL, prochase, NULL))!=0)
   {
    printf("prochase creation failed \n");
    exit(1);
   }
sleep(2); 
}

