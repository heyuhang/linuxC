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
    x[1]=0;
    int k=1;
   	 
    while(k>0){
       x[k]+=1;
       while((x[k]<=n) && !(place(k)))
	   x[k]+=1;
	   if(x[k]<=n)
	       if(k==n)
		  sum++;
	       else{
		  k++;			
		  x[k]=0;
		}
	  else k--;	
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
     printf("%d\n",t);   
     return 0;   
 }  
