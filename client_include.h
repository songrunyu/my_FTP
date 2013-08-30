/*
 * =====================================================================================
 *
 *       Filename:  client_include.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 13时41分35秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>

void my_err(char *string, int line);
int  login();
void command_get(int conn_fd);
void command_sortget(int conn_fd);
void command_put(int conn_fd);
void recv_dir(int conn_fd);
void command_ls(int conn_fd);
void command_watch(int conn_fd);
int  choice_input(int conn_fd);

