#include<stdio.h>

int max(int a,int b)
{
  return a>b?a:b;
}

int min(int a,int b)
{
  return a>b?b:a;
}

void knapsack(int v[],int w[],int c,int n,int m[][11])
{
  int jMax;
  int i,j;

  jMax=min(w[n]-1,c);
  for(j=0;j<=jMax;j++)
    m[n][j]=0;
  for(j=w[n];j<=c;j++)
    m[n][j]=v[n];

  for(i=n-1;i>1;i--){
    jMax=min(w[i]-1,c);
    for(j=0;j<=jMax;j++)
      m[i][j]=m[i+1][j];
    for(j=w[i];j<=c;j++)
      m[i][j]=max(m[i+1][j],m[i+1][j-w[i]]+v[i]);
  }
  m[1][c]=m[2][c];
  if(c>=w[1])
    m[1][c]=max(m[1][c],m[2][c-w[1]]+v[1]);
/*
  for(i=0;i<6;i++){
    for(j=0;j<11;j++)
      printf("%d ",m[i][j]);
    printf("\n");
  }
*/
}

void Traceback(int m[][11],int w[],int c,int n,int x[])
{
  int i,j;

  for(i=0;i<6;i++){
    for(j=0;j<11;j++)
      printf("%d ",m[i][j]);
    printf("\n");
  }
  for(i=1;i<n;i++)
    if(m[i][c]==m[i+1][c])
      x[i]=0;
    else{
      x[i]=1;
      c-=w[i];
    }
  x[n]=(m[n][c])?1:0;
}

int main()
{
  int n=5;
  int c=10;
  int w[]={0,4,2,6,5,4};
  int v[]={0,6,3,5,4,6};
  int m[6][11];
  int x[6];
  int i,sum=0;
  
  knapsack(v,w,c,n,m);
  Traceback(m,w,c,n,x);

  for(i=1;i<=n;i++){
    printf("%d ",x[i]);
    sum+=(x[i]*v[i]);
  }
  printf("\nsum=%d\n",sum);

  return 0;
}
