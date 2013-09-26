#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<malloc.h>

#define PROCESS_NAME_LEN 32   /*进程名长度*/
#define MIN_SLICE    10             /*最小碎片的大小*/
#define DEFAULT_MEM_SIZE 1024     /*内存大小*/
#define DEFAULT_MEM_START 0       /*起始位置*/
#define MA_FF 1
#define MA_BF 2
#define MA_WF 3
int mem_size=DEFAULT_MEM_SIZE; /*内存大小*/
int ma_algorithm = MA_FF;           /*当前分配算法*/
static int pid = 0;                /*初始pid*/
int flag = 0;/*设置内存大小标志*/

//描述每一个空闲块的数据结构
struct free_block_type{
  int size;
  int start_addr;
  struct free_block_type * next;
};

//指向内存中空闲块联表的链表的首指针
struct free_block_type * free_block_head=NULL;
/*每个进程分配到的内存块的描述*/
struct allocated_block{
    int pid;    int size;
    int start_addr;
    char process_name[PROCESS_NAME_LEN];
    struct allocated_block *next;
};
/*进程分配内存块链表的首指针*/
struct allocated_block *allocated_block_head = NULL;

int display_menu();//菜单

//void kill_process(int pid);//终止一个进程
//void free_mem();//释放内存
//void display_mem_usage();//显示内存分配情况
//void do_exit();//退出程序
//初始化空闲块
int display_menu()
{
  return 0;
}

void init_free_block()
{
  free_block_head=(struct free_block_type *)malloc(sizeof(struct free_block_type));
  if(free_block_head==NULL)
    return ;
  free_block_head->size=0;
  free_block_head->start_addr=0;
  free_block_head->next=NULL;
}
//初始化内存块大小
void init_free_block_size(int mem_size)
{
  struct free_block_type * p;
  if(flag==1)
    return ;
  flag=1;
  p=(struct free_block_type *)malloc(sizeof(struct free_block_type));
  if(p==NULL){
    printf("申请失败！\n");
    exit(1);
  }
  p->size=mem_size;
  p->start_addr=DEFAULT_MEM_START;
  p->next=free_block_head->next;
  free_block_head->next=p;
}
//按FF重新整理内存
int rearrange_FF()
{
  struct free_block_type * h,* q,* s;
  h=free_block_head;
  if(h->next==NULL)
    return -1;
  else return 0;
  s=h->next;
  q=s->next;
  //选择排序
  while(h->next->next!=NULL){
    while(q!=NULL){
      if(h->next->start_addr>q->start_addr){
        s->next=q->next;
        q->next=h->next;
        h->next=q;
        q=s->next;
      }else{
        q=q->next;
      }

      h=h->next;
    }
  }
  return 0;
}
//按WF重新整理内存
int rearrange_WF()
{
  struct free_block_type * h,* q,* s,* p;
  h=free_block_head;
  if(h->next==NULL)
    return -1;
  else return 0;
  s=h->next;
  p=s;
  q=s->next;
  //冒泡排序(大到小)
  while(p!=NULL){
    while(q!=NULL){
      if(s->size < q->size){
        s->next=q->next;
        q->next=h->next;
        h->next=q;
        q=s->next;
        h=h->next;
      }else{
        q=q->next;
        s=s->next;
        h=h->next;
      }
      h=p;
      s=h->next;
      q=s->next;
      p=p->next;
    }
  }
  return 0;
}
//按BF重新整理内存
int rearrange_BF()
{
  struct free_block_type * h,* q,* s;
  h=free_block_head;
  if(h->next==NULL)
    return -1;
  else return 0;
  s=h->next;
  q=s->next;
  //选择排序
  while(h->next->next!=NULL){
    while(q!=NULL){
      if(h->next->size > q->size){
        s->next=q->next;
        q->next=h->next;
        h->next=q;
        q=s->next;
      }else{
        q=q->next;
      }

      h=h->next;
    }
  }
  return 0;
}
//设置分配算法
void set_algorithm(int algorithm)
{
  ma_algorithm=algorithm;
}
//创建新进成
int new_process()
{
  pid_t npid;
  npid=ftok();//创建新进成
  pid=getpid();
}

int main()
{

  return 0;
}
