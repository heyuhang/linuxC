#include<stdio.h>
#include<stdlib.h>

#define MAXSIZE 50

typedef struct list
{
  int data[MAXSIZE];
  int last;
} LinkList;

void init(LinkList * head)
{
  head->last=-1;
}

void EnterList(LinkList * head,int middle)
{
  head->last++;
  head->data[head->last]=middle;
}

int BinarySearch(int a[],LinkList * head,int left,int right,int i)
{
  int middle;
 
  if(left>right)
    return ;
    middle=(left+right)/2;
      
    if(a[middle]==middle){
      EnterList(head,middle);
      BinarySearch(a,head,left,middle-1,i);
      BinarySearch(a,head,middle+1,right,i);
    }else if(a[middle]>middle)
      BinarySearch(a,head,left,middle-1,i);
    else BinarySearch(a,head,middle+1,right,i);
}


int main()
{
  int a[MAXSIZE];
  int i=0,j;
  LinkList p;

  printf("):\n");
  for(j=0;j<MAXSIZE;j++){
    scanf("%d",&a[j]);
    if(a[j]==-1)
        break;
  }
  init(&p);
  BinarySearch(a,&p,0,j--,i);
  if(p.last==-1)
  {
    printf("don't find\n");
    return -1;
  }
  while(i<=p.last)
  {
    printf("%d ",p.data[i]);
    i++;
  } 
  printf("\n");

  return 0;
}
