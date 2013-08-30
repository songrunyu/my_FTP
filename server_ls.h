/*
 * =====================================================================================
 *
 *       Filename:  server_ls.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月13日 15时33分24秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */



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
	                strcpy(name[a].filename, entry->d_name);
	                memset(path_buff, '\0', 256);
	                getcwd(path_buff, 256);
	    	    	strcat(path_buff, "/");
	    	    	strcat(path_buff, entry->d_name);
	                strcpy(name[a].path, path_buff);
	                count += statbuff.st_size;
	                a++;
	    }
    }
    chdir("..");                //回到上一级目录
    closedir(dp);

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

