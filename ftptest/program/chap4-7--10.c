/*
 * =====================================================================================
 *
 *       Filename:  test5.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年06月23日 19时58分20秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongbaobei, dlvbaobei@gmail.com
 *        Company:  The Software Class 1203
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int i, j, k = 0, s, n, m;
	int a[80];

	printf("请输入犯人个数:\n");
	scanf("%d", &n);
	printf("请给犯人编号:\n");
	for(i = 1; i <= n; i++)
		scanf("%d", &a[i]);
	printf("从第几个开始数起:\n");
	scanf("%d", &s);
	printf("每次数多少:\n");
	scanf("%d", &m);

	while(k < n-1)
	{
		for(i = 1; i < m; )
		{
loop:			if(s > n)
			      s = 1;
			if(a[s] == 0) 		/*若已经被处死，则跳过该人的报数*/
				s++;
			else
			{
				i++;
				s++;
			}
		}
		if(s > n)
		      s = 1;
		if(a[s] == 0) 		/*判断要处死的人是不是已经被处死*/
		      goto loop;

		printf("第%d个处决的是%d号\n", k+1, a[s]);
		a[s] = 0;
		s++;
		k++;
	}
	for(i = 1; i <= n; i++)
	{
		if(a[i] != 0)
		printf("最后存活的是%d号\n", a[i]);
	}
	return EXIT_SUCCESS;
}

