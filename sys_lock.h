/*
 * =====================================================================================
 *
 *       Filename:  fcntl_lock.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月12日 15时06分38秒
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


int lock_set(int fd, struct flock *lock)
{
	if(fcntl(fd, F_SETLK, lock) == 0)
	{
		if(lock->l_type == F_RDLCK)
		{
			printf("set read lock, pid:%d\n",getpid());
		}
		else if(lock->l_type == F_WRLCK)
		{
			printf("set write lock,pid:%d\n",getpid());
		}
		else if(lock->l_type == F_UNLCK)
		{
			printf("release lock, pid:%d\n",getpid());
		}
	}
	else
	{
		my_err("fcntl",__LINE__);
	}

	return 0;
}

int lock_test(int fd, struct flock *lock)
{
	if(fcntl(fd, F_GETLK, lock) == 0)
	{
		if(lock->l_type == F_UNLCK)
		{
			printf("lock can be set in fd\n");
			return 0;
		}
		else
		{
			if(lock->l_type == F_RDLCK)
			{
				printf("can't set lock, read lock has been set by:%d\n",lock->l_pid);
			}
			else if(lock->l_type == F_WRLCK)
			{
				printf("can't set lock, write lock has been set by:%d\n",lock->l_pid);
			}
			return -2;
		}
	}
	else 
	{
		my_err("fcntl",__LINE__);
	}

}



int f_lock(char ip[20], char command[10], char filename[20])
{
	int 		fd;
	int 		ret;
	char 		buf[256];
	char 		buf_time[32];
	time_t          *ptm;
	struct flock 	lock;

	if((fd = open("/home/songrunyu/my_ftp/log/system_log.txt", O_CREAT|O_RDWR|O_APPEND, 0666)) == -1)
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
	sprintf(buf, "%s %-20s %-20s %-20s\n",buf_time, ip,command,filename);
	write(fd, buf, strlen(buf));

	lock.l_type = F_UNLCK;
	lock_set(fd, &lock);
	close(fd);
	return 0;
	
}
