/*
 * =====================================================================================
 *
 *       Filename:  client_watch.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 13时59分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */


void command_watch(int conn_fd)
{
	float 	count;
	int 	len;
	
	len = recv(conn_fd, &count, sizeof(count), 0);
	if(len == -1)
	{
			my_err("recv",__LINE__);
	}
	printf("该目录下文件总大小为%.4fGB\n",count);
	fflush(stdin);
    	printf("\n按任意键返回主菜单：\n");
   	getchar();
	getchar();
  	system("reset");
   	choice_input(conn_fd);

}


