/*
 * =====================================================================================
 *
 *       Filename:  client_sortget.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 13时57分27秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */


void command_sortget(conn_fd)
{
	int 	m;
	int 	ret;
	int 	num;
	int 	len;
	int 	fd;
	int   	re_len = 1;
	int 	write_len;
	int 	recv_len;
	char 	path[256];
	char 	type[10];
	char 	filename[256];
    	char 	buf[4096];
	char 	getpath[256];

	printf("请输入所要下载文件类型的后缀名（eg:.mp3）:");
	scanf("%s",type);
	getchar();

	ret = send(conn_fd, type, sizeof(type), 0);               //接收类型名
	if(len == -1)
	{
			my_err("send",__LINE__);
        }

	ret = recv(conn_fd, &num, sizeof(num), 0);               //接收文件数量
	if(len == -1)
	{
			my_err("send",__LINE__);
 	}
	printf("所要接收的%s类型文件共%d个\n",type,num);


	m = 0;
	while( m<num )
	{
		memset(filename, '\0', 256);
					
		ret = recv(conn_fd, filename, sizeof(filename), 0);
    		if(ret == -1)
		{
			my_err("recv", __LINE__);
		}
			
		recv(conn_fd, &re_len, sizeof(re_len), 0);             //接收文件大小
	   	printf("%s大小为：%d字节\n", filename,re_len);
	
	    	getcwd(path, 256);     //获取当前目录
	    	mkdir("ftp_get", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); //创建接收文件的目录ftp_get
	
	   	memset(getpath, '\0', 256);
	   	strcat(getpath, path);
	    	strcat(getpath, "/");
	    	strcat(getpath, "ftp_get/");
	    	strcat(getpath, filename);      //文件的绝对路径
	
	    	fd = open(getpath, O_CREAT | O_RDWR, 0666); //创建filename文件
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
				re_len = re_len - recv_len;
			}
			else
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
				re_len = re_len - recv_len;
	    		}
		}
		printf("文件接收完毕！\n\n");
		close(fd);
		m++;
	}
    	printf("所有文件接受完毕，按任意键返回主菜单：\n");
    	getchar();
    	system("reset");
    	choice_input(conn_fd);

}


