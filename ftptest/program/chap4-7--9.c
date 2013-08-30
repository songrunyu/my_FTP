/* =====================================================================================
 * 
 *       Filename:  tes.c
 *        Version:  1.0
 *        Created:  2013年06月18日 16时44分15秒
 *       Revision:  none
 *       Compiler:  g++
 *         Author:  shenzhaomeng, 
 *        Company:  Class 1107 of Computer Science and Technology
 * 
 * =====================================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#define N 256//必须是256个
int main(int argc, char *argv[])
{
	char op[N]={'\0'};//给定的数组
	int po[N];//统计的数组
	int i;
	
	printf("inputs:");
	gets(op);

	for(i=0; i<N; i++)//将统计数组中所有变量赋值为0
	{
		po[i]=0;
	}

	for(i=0; op[i]!='\0'; i++) //将给定数组的元素作为统计数组的下标
	{
		po[(unsigned char)op[i]]++;
	}

	for(i=0;i<N;i++)
	{
		if(po[i]!= 0)
		{
			printf("%c is %d \n",(char)i,po[i]);
		}
	}
	printf("over!\n");

	return 0;
}

