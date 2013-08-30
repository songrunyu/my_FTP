/*
 * =====================================================================================
 *
 *       Filename:  server.c
 *
 *    Description:  服务端
 *
 *        Version:  1.0
 *        Created:  2013年08月05日 10时49分56秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>



#define SERV_PORT 	4507
#define LISTENQ   	5

struct sockaddr_in 	cli_addr;
int lock_set(int fd,  struct flock *lock);
int lock_test(int fd, struct flock *lock);
int f_lock(char ip[], char command[], char filename[]);
int e_lock(char ip[], char err_string[], int line);


struct file
{
    char 	filename[256];
    char 	path[256];

};
struct file name[20];

void my_err( char *err_string, int line)
{
    fprintf(stderr, "line:%d", line);
    perror(err_string);
    e_lock(inet_ntoa(cli_addr.sin_addr), err_string, line);
    exit(1);
}


int 		i = 0;
int 		count;        //该目录下所有文件的大小
void find_file(char *dir)
{
    DIR 		*dp;
    struct dirent 	*entry;
    struct stat 	statbuff;

    if(!(dp = opendir(dir)))
    {
        perror("scan_dir :can't open dir !\n");
        return;
    }

    chdir(dir);
    while((entry = readdir(dp)) != NULL)
    {
        lstat(entry->d_name, &statbuff);
        if(S_IFDIR & statbuff.st_mode)
        {
            if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
                continue;
            find_file(entry->d_name);
        }
        else
        {
            char path_buff[256];
            strcpy(name[i].filename, entry->d_name);
            memset(path_buff, '\0', 256);
            getcwd(path_buff, 256);
	    strcat(path_buff, "/");
	    strcat(path_buff, entry->d_name);
            strcpy(name[i].path, path_buff);
            count += statbuff.st_size;
            i++;
        }
    }
    chdir("..");                //回到上一级目录
    closedir(dp);

}

void choice_watch(int conn_fd)
{
	int 	len;
	float 	size;
	char 	workpath[256] = "/home/songrunyu/ftptest";

	size = (count*1.0)/(1024*1024*1024);
	printf("size:%.4f",size);
	len = send(conn_fd, &size, sizeof(float), 0);
	if(len == -1)
	{
		my_err("send",__LINE__);
	}
	printf("该目录下总文件大小为%.4fGB\n",size);
	f_lock(inet_ntoa(cli_addr.sin_addr), "choice_watch", "\0");
	deal_command(conn_fd);

	
}



void search_file(int k, char *path, char *filename1)
{

    k = 0;
    printf("\nnext print\n");
    while(k < 20)
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


	while(i<20)
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
        		}
        		memset(buff, '\0', 4096);

		}
    		printf("发送完毕\n");
    		close(fd);

	}
	printf("所有文件已发送完毕\n");
	f_lock(inet_ntoa(cli_addr.sin_addr), "sortget", type);
   	deal_command(conn_fd);

}

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
    	deal_command(conn_fd);
}

void  open_dir(char path[256], char filename[4096])
{
    DIR 		*dir;     //目录文件描述符
    struct dirent 	*pdr;     //目录文件的结构体
    struct stat 	buf;

    dir = opendir(path);
    if(dir == NULL)
    {
        my_err("opendir", __LINE__);
    }

    chdir(path);             //切换到当前目录中去

    while((pdr = readdir(dir)) != NULL)
    {
        if(pdr->d_name[0] == '.')    //若为隐藏文件则跳过
        {
            continue;
        }

        lstat(pdr->d_name, &buf) < 0; //获取下一级成员属性

        if(S_IFDIR & buf.st_mode) //是目录则递归调用，继续打开
        {
            strcat(filename, "\n");
            strcat(filename, pdr->d_name);
            strcat(filename, ":\n");

            open_dir(pdr->d_name, filename);
        }
        else                      //是文件则存入filename
        {
            strcat(filename, pdr->d_name);
            strcat(filename, "\n");
        }
    }

    chdir("..");                //回到上一级目录
    closedir(dir);


}


//处理ls命令的函数
void choice_ls(int conn_fd)
{

    DIR 		*dir;
    int 		len;
    char 		filename[256];
    char 		workpath[256] = "/home/songrunyu/ftptest";

    memset(filename, '\0', 256);
    open_dir(workpath, filename);

    len = send(conn_fd, (void *)&filename, sizeof(filename), 0);
    if(len == -1)
    {
        my_err("send", __LINE__);
    }
    printf("文件名已传送完毕！\n");
    f_lock(inet_ntoa(cli_addr.sin_addr), "ls", "\0");
    deal_command(conn_fd);

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
    find_file("/home/songrunyu/ftptest");
    while(1)
    {
        conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);
	f_lock(inet_ntoa(cli_addr.sin_addr), "connect us", "\0");
        if(conn_fd < 0)
        {
            my_err("accept", __LINE__);
        }

        printf("accept a new client,ip:%s\n", inet_ntoa(cli_addr.sin_addr));
        pid = fork();  //创建一个子进程处理连接请求
        switch(pid)
        {
        case 0:
            deal_command(conn_fd);
            break;
        default:
            break;
        }
        if(pid != 0)
        {
            wait(&stat_val);
        }
    }

    close(conn_fd);
    return 0;
}



















