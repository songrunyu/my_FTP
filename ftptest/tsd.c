/*
 * =====================================================================================
 *
 *       Filename:  tsd.c
 *
 *    Description：创建线程的私有数据 
 *
 *        Version:  1.0
 *        Created:  2013年07月30日 11时33分01秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_key_t 	key;

void *thread2(void *arg)
{
	int 	tsd = 5;
	printf("thread %lu is running \n", pthread_self());
	pthread_setspecific(key, (void *)tsd);
	printf("thread %lu returns %d \n",pthread_self(),pthread_getspecific(key));

}

void *thread1(void *arg)
{
	int 	tsd = 0;
	pthread_t 	thid2;

	printf("thread %lu is running \n",pthread_self());
	pthread_setspecific(key, (void *)tsd);
	pthread_create (&thid2, NULL, thread2, NULL);
	sleep(5);
	printf("thread %lu returns %d\n",pthread_self(),pthread_getspecific(key));

}

int main(void)
{
	pthread_t 	thid1;
	printf("main thread begins running \n");
	pthread_key_create (&key, NULL);
	pthread_create (&thid1, NULL, thread1, NULL);
	sleep(8);
	pthread_key_delete(key);
	printf("main thread exit\n");

	return EXIT_SUCCESS;
}

