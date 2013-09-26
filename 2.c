 #include<stdio.h>   
 #define N 15   
   
 int n; //皇后个数   
 int sum = 0; //可行解个数   
 int x[N]; //皇后放置的列数   
   
   
 int place(int k)   
 {   
     int i;   
     for(i=1;i<k;i++)   
       if(abs(k-i)==abs(x[k]-x[i]) || x[k] == x[i])   
         return 0;   
     return 1;   
 }   
   
   
 int queen(int t)   
 {   
   int i;
     if(t>n && n>0) //当放置的皇后超过n时，可行解个数加1，此时n必须大于0   
       sum++;   
     else  
       for( i=1;i<=n;i++)   
       {   
           x[t] = i; //标明第t个皇后放在第i列   
           if(place(t)) //如果可以放在某一位置，则继续放下一皇后   
             queen(t+1);    
       }   
     return sum;   
 }   
   
 int main()   
 {   
     int t;   
     scanf("%d",&n);   
     t = queen(1);   
     if(n == 0) //如果n=0，则可行解个数为0，这种情况一定不要忽略   
       t = 0;   
     printf("%d",t);   
     return 0;   
 }  
