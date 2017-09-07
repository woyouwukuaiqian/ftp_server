 ///
 /// @file    func.c
 /// @author  wx(1418957094@qq.com)
 /// @date    2017-09-06 09:45:34
 ///


#include "include/ftp.h"

int command_handle(char* buf,int new_fd)
{
	char buf1[128]={0};
	char buf2[128]={0};
	if(buf==NULL)
	{
		return -1;
	}
	int i=0;
	while(buf[i]!=' '&&buf[i]!=0&&buf[i]!='\n')
	{
		buf1[i]=buf[i];
		i++;
	}
	i++;
	int j=0;
	while(buf[i]!=0 && buf[i]!=' '&&buf[i]!='\n')
	{
		buf2[j]=buf[i];
		i++;
		j++;
	}
	command cmd;
	memset(&cmd,0,sizeof(cmd));
	if(!strcmp(buf1,"pwd"))
	{
		my_pwd(&cmd);
	}
	if(!strcmp(buf1,"ls"))
	{
		func_ls(&cmd);
	}
	if(!strcmp(buf1,"cd"))
	{
		func_cd(&cmd,buf2);
	}
	if(!strcmp(buf1,"remove"))
	{
		func_remove(&cmd,buf2);
	}
	if(!strcmp(buf1,"puts"))
	{
		printf("buf2=%s\n",buf2);
		func_puts(&cmd,buf2,new_fd);
	}
	if(!strcmp(buf1,"gets"))
	{
//		func_gets(&cmd,buf2,new_fd);
	}
	send(new_fd,&cmd,sizeof(cmd),0);
	return 0;
}
