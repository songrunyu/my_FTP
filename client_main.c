/*
 * =====================================================================================
 *
 *       Filename:  client_main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 13时47分54秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */
#include "client_include.h"
#include "client_ls.h"
#include "client_get.h"
#include "client_sortget.h"
#include "client_put.h"
#include "client_watch.h"

int main(int argc, char *argv[])
{
    struct sockaddr_in 	serv_addr;
    int 			conn_fd;
    int 			serv_port;
    int 			ret;
    char 			addr[20];

    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;

    printf("ip:");       //接收ip
    scanf("%s", addr);
    getchar();
    if((inet_aton(addr, &serv_addr.sin_addr)) == 0)
    {
            my_err("inet_aton", __LINE__);
    }

    printf("sin_port:"); //接收端口号
    scanf("%d", &serv_port);
    if(serv_port < 0 || serv_port > 65535)
    {
            printf("invalid serv_addr.sin_port\n");
    }
    if((serv_addr.sin_port = htons(serv_port)) == 0)
    {
            my_err("htons", __LINE__);
    }

   //创建套接字
    conn_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (conn_fd < 0)
    {
	    my_err("socket", __LINE__);
    }

    if (connect(conn_fd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in)) < 0)
    {
	    my_err("connect", __LINE__);
    }

    system("reset");
    printf("\n\n\tconnect to server successfully!\n");
    choice_input(conn_fd);

}


int choice_input(int conn_fd)
{
    int 	ret;
    int 	choice;

    setbuf(stdin, NULL);
    choice = login();

    ret = send(conn_fd, &choice, sizeof(choice), 0);  //将选项发送到s端
    if(ret < sizeof(choice) || ret == -1)
    {
            printf("发送不成功\n");
            my_err("send", __LINE__);
            return -1;
    }

    switch(choice)
    {
        case 1:
            command_ls(conn_fd);
            break;
        case 2:
            command_get(conn_fd);
            break;
        case 3:
            command_put(conn_fd);
            break;
        case 4:
            close(conn_fd);
            break;
        case 5:
    	    command_watch(conn_fd);
    	    break;
        case 6:
    	    command_sortget(conn_fd);
    	    break;
        default:
    	    printf("error input!\n");
	    printf("按任意键返回\n");
	    getchar();
	    getchar();
	    choice_input(conn_fd);

    	    
        }
}

int login()  //登录选项界面
{
    int 	choice;

    printf("\t\t======================================================\n");
    printf("\t\t1. ls（查询）\n");
    printf("\t\t2. get（下载）\n");
    printf("\t\t3. put（上传）\n");
    printf("\t\t4. quit（退出）\n");
    printf("\t\t5. watch file_size(查看目录文件的总大小)\n");
    printf("\t\t6. sortget(分类下载)\n");
    printf("\n\t\tplease input your choice:");
    scanf("%d", &choice);

    return choice;

}

void my_err(char *string, int line)
{
    fprintf(stderr, "line :%d ", line);
    perror(string);
    exit(1);
}

