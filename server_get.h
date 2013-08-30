/*
 * =====================================================================================
 *
 *       Filename:  server_get.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 15时35分05秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */


void choice_get(int conn_fd)
{
    int 		ret;
    int 		read_len = 1;
    int 		send_len;
    int 		re_len;
    int 		len;
    int 		fd;
    int 		k = 0;
    int 		block = 0;
    char 		*p, *q;
    char 		filename1[256];
    char 		path[256];
    char 		workpath[30] = "/home/songrunyu/ftptest";
    char 		buff[4096];
    struct stat 	buf;
    struct file 	name[100];

    memset(filename1, '\0', sizeof(filename1));
    memset(path, '\0', 256);
    ret = recv(conn_fd, filename1, sizeof(filename1), 0);   //接收到文件名
    if(ret == -1)
    {
            my_err("recv", __LINE__);
    }

    if(filename1[0] != '/')                   //若没有路径，就获取绝对路径，存入path
    {
    
            printf("%s\n", filename1);
            search_file(k, path, filename1);
    }
    else              //有路径则存入path中
    {
            strcpy(path, filename1);
    }

    printf("%s\n", path);

    if((lstat(path, &buf)) == -1)
    {
            my_err("lstat", __LINE__);
    }
    re_len = buf.st_size;              //获取文件属性（大小）
    send(conn_fd, &re_len, sizeof(re_len), 0);

    fd = open(path, O_RDWR, 0666);
    if(fd == -1)
    {
            my_err("open", __LINE__);
    }
    
    while((block = read(fd, buff, sizeof(buff))) > 0)
    {
            if(send(conn_fd, buff, sizeof(buff), 0) < 0)
            {
	                my_err("send",__LINE__);
	    }
            memset(buff, '\0', 4096);
    }
    printf("发送完毕\n");
    close(fd);
    f_lock(inet_ntoa(cli_addr.sin_addr), "get", filename1);
    deal_command(conn_fd);

}





void search_file(int k, char *path, char *filename1)
{

    k = 0;
    printf("\nnext print\n");
    while(k < 50)
    {
            printf("[ %d: %s . %s ]\n", k, name[k].filename, name[k].path);
            if(strcmp(filename1, name[k].filename) == 0)
	    {
		    printf("文件已找到\n");
			    
	      	    strcpy(path, name[k].path);
		    break;
	    }
	    k++;
    }
}
