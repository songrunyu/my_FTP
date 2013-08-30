/*
 * =====================================================================================
 *
 *       Filename:  client.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2013年08月05日 20时50分31秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>

void my_err(char *string, int line)
{
    fprintf(stderr, "line :%d ", line);
    perror(string);
    exit(1);
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
    scanf("%s", filename);
    getchar();

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

	ret = send(conn_fd, type, sizeof(type), 0);
	if(len == -1)
	{
		my_err("send",__LINE__);
	}

	ret = recv(conn_fd, &num, sizeof(num), 0);
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
        		}
        		re_len = re_len - recv_len;
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

void command_ls(int conn_fd)
{
    int 	len;
    int 	ret;
    char 	name[256];
    char 	choice;


    recv_dir(conn_fd);

}

void command_watch(int conn_fd)
{
	float 	count;
	int 	len;
	
	len = recv(conn_fd, &count, sizeof(count), 0);
	if(len == -1)
	{
		my_err("recv",__LINE__);
	}
	printf("该目录下文件总大小为%.4fGB\n",count);
	fflush(stdin);
    	printf("\n按任意键返回主菜单：\n");
   	getchar();
	getchar();
  	system("reset");
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
	exit(0);
    }
}

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
