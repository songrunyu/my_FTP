/*
 * =====================================================================================
 *
 *       Filename:  server_sortget.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 15时36分53秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */



void choice_sortget(int conn_fd)
{
	int 		m,n;
	int 		k = 0, kk;
	int 		i = 0;
	int 		fd;
	int 		ret;
	int 		block;
	int 		re_len;
	int 		len1,len2;
	char 		filename[256];
	char 		type[10];
	char 		newtype[10];
        char 		buff[4096];
    	struct stat 	buf;
	char 		path[10];
		
	ret = recv(conn_fd, type, sizeof(type), 0);
	if(ret == -1)
	{
			my_err("recv",__LINE__);
	}
	len2 = strlen(type);
	memset(newtype, '\0', 256);
	for(m=len2-1,n=0; m >= 0; m--)
	{
			newtype[n] = type[m];
			n++;
	}
	printf("转换后后缀名为:%s\n",newtype);


	while(i<50)
	{
			memset(filename, '\0', 256);
			len1 = strlen(name[i].filename);
			for(m=len1-1,n=0; m>=0; m--)
			{
				filename[n] = name[i].filename[m];
				n++;
			}
			if(strncmp(filename, newtype, len2) == 0) 
			{
				path[k] = i;
				i++;
				k++;
			}
			else
			{
				i++;
			}
	
	}
	printf("共找出文件%d个\n",k);
	
	ret = send(conn_fd, &k, sizeof(k), 0);
	if(ret == -1)
	{
		my_err("send",__LINE__);
	}

	kk = k;
	for(k=0; k<kk; k++)
	{
			ret = send(conn_fd, name[(path[k])].filename, sizeof(name[(path[k])].filename), 0);
			if(ret == -1)
			{
				my_err("send",__LINE__);
			}
			printf("%s  %s\n",name[(path[k])].filename, name[(path[k])].path);
	    		if((lstat(name[(path[k])].path, &buf)) == -1)
	    		{
			       	my_err("lstat", __LINE__);
	    		}
	    		re_len = buf.st_size;              //获取文件属性（大小）
	    		send(conn_fd, &re_len, sizeof(re_len), 0);
			printf("%d\n",re_len);
	

			fd = open(name[(path[k])].path, O_RDWR, 0666);
	    		if(fd == -1)
	    		{
			       	my_err("open", __LINE__);
	   	 	}
	    
	    		while((block = read(fd, buff, sizeof(buff))) > 0)
	    		{
				if(send(conn_fd, buff, block, 0) < 0)
			        {
					my_err("send",__LINE__);
					memset(buff, '\0', 4096);
	 			}
			}
	    		printf("发送完毕\n");
			close(fd);
	
	}
	printf("所有文件已发送完毕\n");
	f_lock(inet_ntoa(cli_addr.sin_addr), "sortget", type);
   	deal_command(conn_fd);

}

