/*
 * =====================================================================================
 *
 *       Filename:  server_main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 15时22分26秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */

#include "server_include.h"
#include "server_ls.h"
#include "server_get.h"
#include "server_sortget.h"
#include "server_put.h"
#include "server_watch.h"
#include "sys_lock.h"
#include "err_lock.h"

int main()
{
    int 			sock_fd, conn_fd;
    int 			optval;
    int 			stat_val;
    pid_t 			pid;
    socklen_t 			cli_len;
    struct sockaddr_in 		serv_addr;
    socklen_t 			len;


    //创建套接字
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
	    my_err("socket", __LINE__);
    }
    
    //设置套接字可以重新绑定端口
    optval = 1;
    if ((setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int))) < 0)
    {
	    my_err("setsockopt" , __LINE__);
    }
    
    //初始化s端地址结构
    memset(&serv_addr, 0, sizeof(struct sockaddr_in) );
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//绑定到任意网络接口
  
    //绑定
    len = sizeof(struct sockaddr);
    if(bind(sock_fd, (struct sockaddr *)&serv_addr, len) < 0)
    {
	    my_err("bind", __LINE__);
    }
    
    //监听
    if(listen(sock_fd, LISTENQ) < 0)
    {
	    my_err("listen", __LINE__);
    }
   
    //接受客户端的连接
    cli_len = sizeof(struct sockaddr_in);                
   // find_file("/home/songrunyu/ftptest");
    while(1)
    {
	    conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);
	    f_lock(inet_ntoa(cli_addr.sin_addr), "connect us", "\0");
            find_file("/home/songrunyu/ftptest");
	    if(conn_fd < 0)
	    {
		    my_err("accept", __LINE__);
	    }
	    printf("accept a new client,ip:%s\n", inet_ntoa(cli_addr.sin_addr));
	    if((pid = fork()) == 0)
	    {
		    deal_command(conn_fd);
	    }
    }
    close(conn_fd);
    return 0;
}

//接收选项及文件名
int  deal_command(int conn_fd)
{
	int 	ret;	
    	int 	choice;

    	if((ret = recv(conn_fd, &choice, sizeof(choice), 0)) < 0)  //接收选项到choice中
    	{
            my_err("recv", __LINE__);
    	}

   	if(ret == 0)
    	{
            printf("未接收到命令!\n");
            return 0;
    	}

    	deal_choice(conn_fd, choice);
    	return 0;
}

//处理选项函数
void deal_choice(int conn_fd, int choice)
{
    switch(choice)
    {
        case 1:
            choice_ls(conn_fd);
            break;
        case 2:
            choice_get(conn_fd);
            break;
        case 3:
            choice_put(conn_fd);
            break;
        case 4:
            printf("link break!\n");
            close(conn_fd);
            f_lock(inet_ntoa(cli_addr.sin_addr), "quit", "\0");
            exit(0);
        case 5:
    	    choice_watch(conn_fd);
    	    break;
        case 6:
    	    choice_sortget(conn_fd);
    	    break;
        }
}

void my_err( char *err_string, int line)
{
    fprintf(stderr, "line:%d", line);
    perror(err_string);
    e_lock(inet_ntoa(cli_addr.sin_addr), err_string, line);
    exit(1);
}

