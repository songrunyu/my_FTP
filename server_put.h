/*
 * =====================================================================================
 *
 *       Filename:  server_put.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 15时39分28秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */



void choice_put(int conn_fd)
{
    char  		workpath[256] = "/home/songrunyu/ftptest/put/";
    int 		ret;
    int 		write_len;
    int 		re_len;
    int 		recv_len = 1;
    int 		i, j;
    int 		fd;
    char 		buf[4096];
    char 		path[256];
    char 		filename[256];

    memset(path, '\0', 256);
    ret = recv(conn_fd, path, sizeof(path), 0);
    for(i = 0, j = 0; i < strlen(path); i++)  //从路径中解析出文件名
    {
            if(path[i] == '/')
            {
	                j = 0;
	                continue;
	    }
            filename[j] = path[i];
            j++;
    }
    filename[j] = '\0';
    printf("%s\n", filename);

    memset(path, '\0', 256);
    strcat(path, workpath);
    strcat(path, filename);

    fd = open(path, O_CREAT | O_RDWR, 0666);
    if(fd == -1)
    {
            my_err("open", __LINE__);
    }

    recv(conn_fd, &re_len, sizeof(re_len), 0);
    printf("该文件大小为%d字节\n", re_len);

    while(re_len != 0)
    {
    
    	if(re_len > 4096)
    	{
			memset((void *)&buf, '\0', 4096);
	        	recv_len = recv(conn_fd, buf, 4096, 0);
	
	        	if(recv_len == -1)
	        	{
		            	my_err("recv", __LINE__);
			}
	        	printf("已接受到%d字节\n", recv_len);
	
	        	write_len = write(fd, buf, recv_len);
	        	if(write_len == -1)
	        	{
		           	my_err("write", __LINE__);
	        	}
	}
    	else
    	{
	    		memset((void *)&buf, '\0', 4096);
	    		recv_len = recv(conn_fd, buf, re_len, 0);
	
	    		if(recv_len == -1)
	    		{
				my_err("recv", __LINE__);
			}
	    		printf("已接受到%d字节\n", recv_len);
	
	    		write_len = write(fd, buf, recv_len);
	    		if(write_len == -1)
	    		{
	        		my_err("write", __LINE__);
			}
	
	}
    	re_len = re_len - recv_len;
   }
    printf("文件已接收\n");
    f_lock(inet_ntoa(cli_addr.sin_addr), "put", filename);
    find_file(workpath);
    printf("~~~~~~~~~~~~\n");
    printf("count:%f\n",(count*1.0)/(1024*1024*1024));
    deal_command(conn_fd);
}

