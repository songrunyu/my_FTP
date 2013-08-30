#include<stdio.h>


int main(void)
{
 int x, i;

 for(i=1; i<=100; i++)
   {     
      x=7*i;
  
      if(((x%2)==1) && ((x%3)==2) && ((x%5)==4) && ((x%6)==5))
	{  
           printf("满足要求的长阶的阶梯数至少为：%d\n",x);
	
	}
   }


  if(i>700)  
    printf("在1--700之间没有满足要求的长阶梯\n");
     
} 
 
 

