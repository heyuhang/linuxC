#include<stdio.h>

int max(int a,int b)
{
  return a>b?a:b;
}

int min(int a,int b)
{
  return a>b?b:a;
}

void knapsack(int v[],int w[],int b[],int d,int c,int n,int  m[][11][11])
{
  int jMax,kMax;
  int i,j,k;

  jMax=min(w[n]-1,c);
  kMax=min(b[n]-1,d);
  for(j=0,k=0;j<=jMax || k<=kMax;j++,k++)
      m[n][j][k]=0;
  for(j=w[n];j<=c;j++)
    for(k=b[n];k<=d;k++)
        m[n][j][k]=v[n];

  for(i=n-1;i>1;i--){
    jMax=min(w[i]-1,c);
    kMax=min(b[i]-1,d);
    for(j=0,k=0;j<=jMax || k<=kMax;j++,k++)
        m[i][j][k]=m[i+1][j][k];
    for(j=w[i];j<=c;j++)
       for(k=b[i];k<=d;k++)
          m[i][j][k]=max(m[i+1][j][k],m[i+1][j-w[i]][k-b[i]]+v[i]);
  }
  m[1][c][d]=m[2][c][d];
  if(c>=w[1] && d>=b[i])
    m[1][c][d]=max(m[1][c][d],m[2][c-w[1]][d-b[1]]+v[1]);
/*
  for(i=0;i<6;i++){
    for(j=0;j<11;j++)
      printf("%d ",m[i][j]);
    printf("\n");
  }
*/
}

void Traceback(int m[][11][11],int w[],int b[],int d,int c,int n,int x[])
{
  int i,j,k;
/*
  for(i=0;i<6;i++){
    for(j=0;j<11;j++){
      for(k=0;k<11;k++)
         printf("%d ",m[i][j][k]);
      printf("\n");
    }
    printf("\n");
  }
*/
  for(i=1;i<n;i++)
    if(m[i][c][d]==m[i+1][c][d])
      x[i]=0;
    else{
      x[i]=1;
      c-=w[i];
      d-=b[i];
    }
  x[n]=(m[n][c][d])?1:0;
}

int main()
{
  int n=5;
  int c=10;
  int d=10;
  int w[]={0,3,2,6,5,5};
  int v[]={0,6,3,5,4,6};
  int b[]={0,2,2,6,5,7};
  int m[6][11][11];
  int x[6];
  int i,sum=0;
  
  knapsack(v,w,b,d,c,n,m);
  Traceback(m,w,b,d,c,n,x);

  for(i=1;i<=n;i++){
    printf("%d ",x[i]);
    sum+=(x[i]*v[i]);
  }
  printf("\nsum=%d\n",sum);

  return 0;
}
