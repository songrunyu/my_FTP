/*
 * =====================================================================================
 *
 *       Filename:  client_put.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 13时58分20秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */


void command_put(int conn_fd)
{
    char 		filename[256];
    int 		ret;
    int 		block = 0;
    int 		re_len;
    int 		read_len = 1;
    int 		send_len;
    int 		flag = 1;
    int 		fd;
    char 		buff[4096];
    struct stat 	buf;

    printf("请输入所要上传的文件名(绝对路径)：\n");
    getchar();
    gets(filename);
    send(conn_fd, filename, sizeof(filename), 0);

    if((lstat(filename, &buf)) == -1)
    {
            my_err("lstat", __LINE__);
    }
    re_len = buf.st_size;
    printf("该文件大小为：%d\n", re_len);
    send(conn_fd, &re_len, sizeof(re_len), 0);

    fd = open(filename, O_RDWR, 0666);
    if(fd == -1)
    {
            my_err("open", __LINE__);
    }


    while((block = read(fd, buff, 4096)) > 0)
    {
         	if(send(conn_fd, buff, block , 0) < 0)
    		{
			my_err("send",__LINE__);
		}
    	printf("已发送%d字节\n",block);
    	memset(buff, '\0', 4096);
    }
     
    printf("发送完毕\n");
    close(fd);

    printf("按任意键返回主菜单：\n");
    getchar();
    system("reset");
    choice_input(conn_fd);


}

