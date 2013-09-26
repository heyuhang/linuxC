#include<stdio.h>
  
int w[]={2,4,3,7,5},x[5]={0};
int c=10;
int n=5;
int bestw=0,cw=0;

void Backtrack(int i)
{
  //搜索第i层
  if(i>n){
    if(cw>bestw)
      bestw=cw;
    return ;
  }
  //搜索子树
  if(cw+w[i]<=c){
    x[i]=1;
    cw+=w[i];
    Backtrack(i+1);
    cw-=w[i];
  }
  x[i]=0;
  Backtrack(i+1);
}

int main()
{


  Backtrack(1);

  printf("bestw=%d\n",bestw);

  return 0;
}
