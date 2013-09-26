#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

#define PROCESS_NAME_LEN 32   /*进程名长度*/
#define MIN_SLICE    10             /*最小碎片的大小*/
#define DEFAULT_MEM_SIZE 1024     /*内存大小*/
#define DEFAULT_MEM_START 0       /*起始位置*/
/* 内存分配算法 */
#define MA_FF 1
#define MA_BF 2
#define MA_WF 3
int mem_size=DEFAULT_MEM_SIZE; /*内存大小*/
int ma_algorithm = MA_FF;           /*当前分配算法*/
static int pid = 0;                                      /*初始pid*/
int flag = 0;                             /*设置内存大小标志*/
int block_num=1;  /*空闲分区链的长度*/   

struct free_block_type{
    int size;
    int start_addr;
    struct free_block_type *next;
};  
/*指向内存中空闲块链表的首指针*/
struct free_block_type *free_block;

struct allocated_block{
    int pid;    int size;
    int start_addr;
    char process_name[PROCESS_NAME_LEN];
    struct allocated_block *next;
};
/*进程分配内存块链表的首指针*/
struct allocated_block *allocated_block_head = NULL;



/*初始化空闲块，默认为一块，可以指定大小及起始地址*/
struct free_block_type* init_free_block(int mem_size){
    struct free_block_type *fb;

    fb=(struct free_block_type *)malloc(sizeof(struct free_block_type));
    if(fb==NULL){
            printf("No mem\n");
                    return NULL;
                            }
        fb->size = mem_size;
            fb->start_addr = DEFAULT_MEM_START;
                fb->next = NULL;
                    return fb;
}

/*显示菜单*/
display_menu(){
    printf("\n");
    printf("1 - Set memory size (default=%d)\n", DEFAULT_MEM_SIZE);
    printf("2 - Select memory allocation algorithm\n");
    printf("3 - New process \n");
    printf("4 - Terminate a process \n");
    printf("5 - Display memory usage \n");
    printf("0 - Exit\n");
}

/*设置内存的大小*/
int set_mem_size(struct free_block_type * pre){
    int size;
    if(flag!=0 && block_num<=0){  //防止重复设置
        printf("Cannot set memory size again\n");
        return 0;
    }
    printf("Total memory size =");
    scanf("%d", &size);
    if(size>0) {
         mem_size = size;
         pre->size = mem_size;
    }
    flag=1;  
    return 1;                    
}
/*按指定的算法整理内存空闲块链表*/
void rearrange(int algorithm){
    switch(algorithm){
            case MA_FF:  rearrange_FF(); break;
            case MA_BF:  rearrange_BF(); break;
            case MA_WF: rearrange_WF(); break;
            }
}
/* 设置当前的分配算法 */
void set_algorithm(){
    int algorithm;
    printf("\t1 - First Fit\n");
    printf("\t2 - Best Fit \n");
    printf("\t3 - Worst Fit \n");
    scanf("%d", &algorithm);
    if(algorithm>=1 && algorithm <=3)  
    ma_algorithm=algorithm;
   //按指定算法重新排列空闲区链表
   rearrange(ma_algorithm); 
}
void clear(){
    struct free_block_type *fbt, *pre;
    pre=fbt=free_block;
    if(free_block->size==0){
        free(free_block);
        free_block=pre->next;
    }
    while(pre->next!=NULL){
        fbt=pre->next;
        if(fbt->size==0){
          pre->next=fbt->next;
          free(fbt);
	}
        pre=pre->next;
    }
}
/*分配内存模块*/
int allocate_mem(struct allocated_block *ab){
    struct free_block_type *fbt, *pre;
    int all_pieces_size=0,request_size=ab->size,pre_size;
    fbt = pre = free_block;
    while(pre!=NULL){
      if(pre->size>=request_size){
        pre->size=pre->size-request_size;
        ab->start_addr=pre->start_addr;
        pre->start_addr+=request_size;
	rearrange(ma_algorithm);
        return 1;
      }
      pre=pre->next;
    }
    pre=fbt=free_block;
    all_pieces_size+=fbt->size;
    pre_size=free_block->start_addr;
    while(pre->next!=NULL){
        fbt=pre->next;
        all_pieces_size+=fbt->size;
        pre->next=fbt->next;
        if(all_pieces_size>request_size){
          pre->size=all_pieces_size-request_size;
          if(pre->next!=NULL)
           pre->next->start_addr=pre->start_addr+all_pieces_size;
          pre->start_addr=pre_size+request_size; 
          ab->start_addr=pre_size; 
       	  rearrange(ma_algorithm);  
          return 1;
        }
    }
    return -1;
    //根据当前算法在空闲分区链表中搜索合适空闲分区进行分配，分配时注意以下情况：
    //    // 1. 找到可满足空闲分区且分配后剩余空间足够大，则分割
    //        // 2. 找到可满足空闲分区且但分配后剩余空间比较小，则一起分配
    //            // 3. 找不可满足需要的空闲分区但空闲分区之和能满足需要，则采用内存紧缩技术，进行空闲分区的合并，然后再分配
    //                // 4. 在成功分配内存后，应保持空闲分区按照相应算法有序
    //                    // 5. 分配成功则返回1，否则返回-1
   //                        请自行补充。。。。。
   
}
/*创建新的进程，主要是获取内存的申请数量*/
int new_process(){
    struct allocated_block *ab;
    int size;    int ret;
    ab=(struct allocated_block *)malloc(sizeof(struct allocated_block));
    if(!ab) exit(-5);
    ab->next = NULL;
    pid++;
    sprintf(ab->process_name, "PROCESS-%02d", pid);
    ab->pid = pid;    
    printf("Memory for %s:", ab->process_name);
    scanf("%d", &size);
    if(size>0)  ab->size=size;
      ret = allocate_mem(ab);  /* 从空闲区分配内存，ret==1表示分配ok*/
    /*如果此时allocated_block_head尚未赋值，则赋值*/
    if((ret==1) &&(allocated_block_head == NULL)){ 
      allocated_block_head=ab;
      return 1;        
    }
   /*分配成功，将该已分配块的描述插入已分配链表*/
    else if (ret==1) {
      ab->next=allocated_block_head;
      allocated_block_head=ab;
      return 2;        
    }else if(ret==-1){ /*分配不成功*/
      printf("Allocation fail\n");//
      free(ab);
      return -1;       
    }
    return 3;
}

//找出指定pid的节点
struct allocated_block * find_process(int pid){
    struct allocated_block * ab;
    ab=allocated_block_head;
    while(ab!=NULL){
        if(ab->pid==pid)
           break;
        ab=ab->next;
    }
    return ab;
}
/*删除进程，归还分配的存储空间，并删除描述该进程内存分配的节点*/
void kill_process(){
    struct allocated_block *ab;
    int pid;
    printf("Kill Process, pid=");
    scanf("%d", &pid);
    ab=find_process(pid);
    if(ab!=NULL){
        free_mem(ab); /*释放ab所表示的分配区*/
        dispose(ab);  /*释放ab数据结构节点*/
    }
}
/*将ab所表示的已分配区归还，并进行可能的合并*/
int free_mem(struct allocated_block *ab){
    int algorithm = ma_algorithm;
    struct free_block_type *fbt, *pre, *work;
    fbt=(struct free_block_type*) malloc(sizeof(struct free_block_type));
    if(!fbt) return -1;
    fbt->size=ab->size;
    fbt->next=NULL;
    fbt->start_addr=ab->start_addr;
    pre=free_block;
    while(pre->next!=NULL){
       pre=pre->next;
    }
    pre->next=fbt;
    rearrange_FF();
    pre=free_block;
    while(pre!=NULL && pre->next!=NULL){
      fbt=pre->next;
      if( (pre->size+fbt->size)<=mem_size){
         pre->size+=fbt->size;
         pre->next=fbt->next;
      }else pre=pre->next;
    }
    rearrange(ma_algorithm); 
    return 1;
    // 进行可能的合并，基本策略如下
    //     // 1. 将新释放的结点插入到空闲分区队列末尾
    //         // 2. 对空闲链表按照地址有序排列
    //             // 3. 检查并合并相邻的空闲分区
    //                 // 4. 将空闲链表重新按照当前算法排序
    //                     请自行补充……
    //                         return 1;
                             
}
void do_exit(){
   exit(0);
}
/*释放ab数据结构节点*/
int dispose(struct allocated_block *free_ab){
    struct allocated_block *pre, *ab;
   if(free_ab == allocated_block_head) { /*如果要释放第一个节点*/
        allocated_block_head = allocated_block_head->next;
           free(free_ab);
           return 1;
           }
    pre = allocated_block_head;  
    ab = allocated_block_head->next;
    while(ab!=free_ab){ pre = ab;  ab = ab->next; }
    pre->next = ab->next;
    free(ab);
    return 2;
}
/* 显示当前内存的使用情况，包括空闲区的情况和已经分配的情况 */
int display_mem_usage(){
    struct free_block_type *fbt=free_block;
    struct allocated_block *ab=allocated_block_head;
    if(fbt==NULL) return(-1);
    printf("----------------------------------------------------------\n");
    
        /* 显示空闲区 */
    printf("Free Memory:\n");
    printf("%20s %20s\n", "      start_addr", "       size");
    while(fbt!=NULL){
       printf("%20d %20d\n", fbt->start_addr, fbt->size);
       fbt=fbt->next;
                                        
    }
    /* 显示已分配区 */
    printf("\nUsed Memory:\n");
    printf("%10s %20s %10s %10s\n", "PID", "ProcessName", "start_addr", " size");
    while(ab!=NULL){
       printf("%10d %20s %10d %10d\n", ab->pid, ab->process_name, ab->start_addr, ab->size);
       ab=ab->next;
    }
    printf("----------------------------------------------------------\n");
    return 0;
                            
}
//按FF重新整理内存
int rearrange_FF()
{
    struct free_block_type * h,* q;
    h=free_block;
  if(h->next==NULL)
    return -1;
  for(h;h!=NULL;h=h->next)
    for(q=h->next;q!=NULL;q=q->next)
       if(h->start_addr>q->start_addr){
          h->start_addr=h->start_addr+q->start_addr;
          q->start_addr=h->start_addr-q->start_addr;
          h->start_addr=h->start_addr-q->start_addr;
          h->size=h->size+q->size;
          q->size=h->size-q->size;
          h->size=h->size-q->size;
       }
  return 0;
}
//按WF重新整理内存
int rearrange_WF()
{
    struct free_block_type * h,* q;
    h=free_block;
  if(h->next==NULL)
    return -1;
  for(h;h!=NULL;h=h->next)
    for(q=h->next;q!=NULL;q=q->next)
       if(h->size<q->size){
          h->start_addr=h->start_addr+q->start_addr;
          q->start_addr=h->start_addr-q->start_addr;
          h->start_addr=h->start_addr-q->start_addr;
          h->size=h->size+q->size;
          q->size=h->size-q->size;
          h->size=h->size-q->size;
       }
  return 0;
}
//按BF重新整理内存
int rearrange_BF()
{
    struct free_block_type * h,* q;
    h=free_block;
  if(h->next==NULL)
    return -1;
  for(h;h!=NULL;h=h->next)
    for(q=h->next;q!=NULL;q=q->next)
       if(h->size>q->size){
          h->start_addr=h->start_addr+q->start_addr;
          q->start_addr=h->start_addr-q->start_addr;
          h->start_addr=h->start_addr-q->start_addr;
          h->size=h->size+q->size;
          q->size=h->size-q->size;
          h->size=h->size-q->size;
       }
  return 0;
}

main(){
    char choice;      pid=0;
    struct free_block_type * pre,* fbt;
    int addr=DEFAULT_MEM_START;

    printf("初始化可用表长度(>1)：\n");
    scanf("%d",&block_num);
    free_block = init_free_block(mem_size); //初始化空闲区
    pre=free_block;
    block_num--;
    set_mem_size(pre);
    fbt=free_block;
    while(block_num>0){
       addr=pre->size+pre->start_addr;
       pre = init_free_block(mem_size); //初始化空闲区
       pre->start_addr=addr;
       set_mem_size(pre);
       fbt->next=pre;
       fbt=pre;
       block_num--;
    }
    while(1) {
        display_menu(); //显示菜单
        fflush(stdin);
        choice=getchar(); //获取用户输入
        switch(choice){
          case '1':// set_mem_size(); break;
          case '2': set_algorithm();flag=1; break;
          case '3': new_process(); flag=1; break;
          case '4': kill_process(); flag=1; break;
          case '5': display_mem_usage();    
                          flag=1; break; 
          case '0':do_exit();exit(0);
          default: break;      
        }    
    } 
}
