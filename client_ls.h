/*
 * =====================================================================================
 *
 *       Filename:  client_ls.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 13时54分10秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  software engineering 1203
 *
 * =====================================================================================
 */


void command_ls(int conn_fd)
{
    int 	len;
    int 	ret;
    char 	name[256];
    char 	choice;


    recv_dir(conn_fd);

}

void recv_dir(int conn_fd)
{
    char 	filename[4096];
    int 	len;
    int 	ret;

    memset((void *)&filename, '\0', sizeof(filename));

    len =  recv(conn_fd, (void *)&filename, sizeof(filename), 0 );
    if(len == -1)
    {
            printf("接收文件错误\n");
            my_err("recv", __LINE__);
    }

    printf("%s", filename);

    printf("\n接收完毕！\n\n\n");
    printf("按任意键返回主菜单：\n");
    getchar();
    getchar();
    system("reset");
    choice_input(conn_fd);

}
