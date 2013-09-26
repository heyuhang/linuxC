#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <signal.h>
#include <ctype.h> 
#include<stdlib.h>

#define MAX_CHILD_NUMBER 10 

#define SLEEP_INTERVAL 2 
int proc_number=0;
void do_something();

main(int argc, char* argv[],char ** environ)
{
      int child_proc_number = MAX_CHILD_NUMBER; 
      int i, ch; 
      pid_t  child_pid; 
      pid_t pid[10]={0};
     if (argc > 1)
     {
         child_proc_number = atoi(argv[1]); 
         child_proc_number= (child_proc_number > 10) ? 10 :
                                                    child_proc_number;
     } 
    for (i=0; i<child_proc_number; i++) {
        child_pid=fork();
        switch(child_pid){
          case -1:exit(1);

          case 0:proc_number=i;
                 do_something();
                 exit(1);
          default:pid[i]=child_pid;break;
        
      }
    }
      /* 让用户选择杀死进程，数字表示杀死该进程，q退出 */
   while ((ch = getchar()) != 'q'){ 
        if (isdigit(ch)){
                 kill(pid[ch-'0'],SIGTERM);
                 wait();
        }
   }
   kill(0,SIGTERM);
   return;
}

void do_something() { 
     for(;;){  
         printf("This is process No.%d and its pid is %d\n", 
                   proc_number,  getpid());
         sleep(SLEEP_INTERVAL); /* 主动阻塞两秒钟 */
     }
} 
