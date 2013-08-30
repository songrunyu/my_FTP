/*
 * =====================================================================================
 *
 *       Filename:  server_watch.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 15时40分26秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */


void choice_watch(int conn_fd)
{
	int 	len;
	float 	size;
	char 	workpath[256] = "/home/songrunyu/ftptest";

	size = (count*1.0)/(1024*1024*1024);
	printf("size:%.4f",size);
	len = send(conn_fd, &size, sizeof(float), 0);
	if(len == -1)
	{
			my_err("send",__LINE__);
	}
	printf("该目录下总文件大小为%.4fGB\n",size);
	f_lock(inet_ntoa(cli_addr.sin_addr), "choice_watch", "\0");
	deal_command(conn_fd);	
 	
}


