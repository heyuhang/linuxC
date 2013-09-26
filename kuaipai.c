#include<stdio.h>

//产生300以内的随机数
int rabs(int n)
{
  int k,l,m,i;

  k=300+1;
  l=2;
  while(l<k)
    l+=l;
  m=4*l;
  k=*r;
  i=0;
  while(i<=n-1){
    k=k+k+k+k+k;
    k=k%m;
    l=k/4+300;
    if(l<=0){
      p[i]=1;
      i=i+1;
    }
  }

  return k;
}

void main()
{
  int a[10];
  int i,j,t;

  for(i=0;i<100;i++){
    a[i]=rabs(100);
    if(i%10==0)
      printf("%d ",a[i]);
    printf("\n");
  }
  
}
