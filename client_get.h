/*
 * =====================================================================================
 *
 *       Filename:  client_get.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 13时56分24秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */


void command_get(int conn_fd)
{
    int 		ret;
    int 		i = 0;
    int 		j = 0;
    int 		recv_len = 1;
    int 		re_len;
    int 		write_len;
    char 		newfilename[256];
    char 		filename[256];
    char 		temp[256];
    char 		path[256];
    char 		getpath[256];
    char 		*file;
    char 		buf[4096];
    int  		fd; //文件描述符

    printf("\n\n\t请输入要下载的文件名：\n");
    getchar();
    gets(filename);

    ret = send(conn_fd, filename, sizeof(filename), 0);
    if(ret == -1)
    {
            my_err("send", __LINE__);
    
    }

    if(filename[0] == '/')
    {
            memset((void *)&newfilename, '\0', sizeof(filename));  //从路径中解析出文件名
            for(i = 0, j = 0; i < strlen(filename); i++)
            {
	                if(filename[i] == '/')
	                {
			                j = 0;
			                continue;
		        }
	                newfilename[j] = filename[i];
	                j++;
	    }
            newfilename[j] = '\0';
    }
    else
    {
            strcpy(newfilename, filename);
    }

    recv(conn_fd, &re_len, sizeof(re_len), 0);             //接收文件大小
    printf("该文件大小为：%d字节\n", re_len);

    getcwd(path, 256);     //获取当前目录
    if(path == NULL)
    {
            my_err("getcwd", __LINE__);
    }
    mkdir("ftp_get", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); //创建接收文件的目录ftp_get

    memset(getpath, '\0', 256);
    strcat(getpath, path);
    strcat(getpath, "/");
    strcat(getpath, "ftp_get/");
    strcat(getpath, newfilename);      //文件的绝对路径

    fd = open(getpath, O_CREAT | O_RDWR, 0666); //创建newfilename文件
    if(fd == -1)
    {
            my_err("open", __LINE__);
    }

    while(re_len != 0)
    {
            if(re_len > sizeof(buf))
            {
	                memset((void *)&buf, '\0', 4096);
	                recv_len = recv(conn_fd, buf, 4096, 0);  //将文件写入缓存
	    
	                if(recv_len == -1)
	                {
			                my_err("recv", __LINE__);
			            }
	                printf("已接收%d字节\n", recv_len);
	    
	                write_len = write(fd, buf, recv_len);
	                if(write_len == -1)
	                {
			                my_err("write", __LINE__);
			            }
	            }
            else                                                  //最后一次接收
            {
	                memset((void *)&buf, '\0', 4096);
	                recv_len = recv(conn_fd, buf, re_len, 0);  //将文件写入缓存
	    
	                if(recv_len == -1)
	                {
			                my_err("recv", __LINE__);
			}
	                printf("已接收%d字节\n", recv_len);
	    
	                write_len = write(fd, buf, recv_len);
	                if(write_len == -1)
	                {
			                my_err("write", __LINE__);
			}
	    }
            re_len = re_len - recv_len;
    }
    printf("\n文件接收完毕！\n\n");
    close(fd);
    printf("按任意键返回主菜单：\n");
    getchar();
    system("reset");
    choice_input(conn_fd);
}

