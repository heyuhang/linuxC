#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <ctype.h>
#include <pthread.h> 

#define MAX_THREAD 3 
unsigned long long main_counter, counter[MAX_THREAD]; 
/* unsigned long  long是比long还长的整数 */
void* thread_worker(void*); 
pthread_mutex_t  mutex;
int main(int argc, char* argv[])
{ 
      int i, rtn, ch; 
      pthread_t pthread_id[MAX_THREAD] = {0}; /* 存放线程id*/ 
      for (i=0; i<MAX_THREAD; i++){ 
        pthread_create(&pthread_id[i],NULL,thread_worker,&i);
          
      } 
      do {/* 用户按一次回车执行下面的循环体一次。按q退出 */ 
           unsigned long long sum = 0;
           for (i=0; i<MAX_THREAD; i++){/* 求所有counter的和 */
                sum += counter[i];
               printf("%d  ",i); 
                printf("%llu  ", counter[i]); 
           }
           printf("%llu/%llu\n", main_counter, sum);
      }  while ((ch = getchar()) != 'q'); 
      return 0; 
}

void* thread_worker(void* p) { 
    int thread_num;  
    thread_num=* (int *)p;
    
    for(;;) { /* 无限循环 */
               counter[thread_num]++; /* 本线程的counter加一 */ 
               main_counter++; /* 主counter 加一 */
     }
}
