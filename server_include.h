/*
 * =====================================================================================
 *
 *       Filename:  server_include.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 15时12分18秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#define SERV_PORT 	4507
#define LISTENQ   	10

struct sockaddr_in 	cli_addr;
int 			a = 0;
int 			count;  

struct file
{
    char 	filename[256];
    char 	path[256];
};
struct file name[50];

int   lock_set(int fd,  struct flock *lock);
int   lock_test(int fd, struct flock *lock);
int   f_lock(char ip[], char command[], char filename[]);
int   e_lock(char ip[], char err_string[], int line);
void  my_err(char *err_string, int line);
void  find_file(char *dir);
void  choice_watch(int conn_fd);
void  search_file(int k, char *path, char *filename1);
void  choice_get(int conn_fd);
void  choice_sortget(int conn_fd);
void  choice_put(int conn_fd);
void  open_dir(char path[256], char filename[4096]);
void  choice_ls(int conn_fd);
void  deal_choice(int conn_fd, int choice);
int   deal_command(int conn_fd);
