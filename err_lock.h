/*
 * =====================================================================================
 *
 *       Filename:  err_lock.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 11时13分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<time.h>



int e_lock(char ip[20], char err_string[20], int line)
{
	int 		fd;
	int 		ret;
	char 		buf[256];
	char 		buf_time[32];
	time_t          *ptm;
	struct flock 	lock;

	if((fd = open("/home/songrunyu/my_ftp/log/error_log.txt", O_CREAT|O_RDWR|O_APPEND, 0666)) == -1)
	{
			my_err("open",__LINE__);
	}

	memset(&lock, 0, sizeof(struct flock));
	lock.l_start = SEEK_SET;
	lock.l_whence = 0;
	lock.l_len = 0;

	lock.l_type = F_WRLCK;
	if(lock_test(fd, &lock) == 0)
	{
			lock.l_type = F_WRLCK;
			lock_set(fd, &lock);
	}

	ptm = (time_t *)malloc(sizeof(time_t));
	time(ptm);
	memset(buf, 0, 256);
	strcpy(buf_time, ctime(ptm));
	buf_time [strlen (buf_time) - 1] = '\0';
	sprintf(buf, "%s %-20s %-20s %-20d\n",buf_time, ip,err_string,line);
	write(fd, buf, strlen(buf));

	lock.l_type = F_UNLCK;
	lock_set(fd, &lock);
	close(fd);
	return 0;
	
}
