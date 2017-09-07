 ///
 /// @file    child.c
 /// @author  wx(1418957094@qq.com)
 /// @date    2017-09-05 12:35:43
 ///

#include "include/head.h"
#include "include/ftp.h"
void child(int spfd)
{

	int new_fd;
	int ret;
	char c[128]={0};
	chdir("/home/wx/ftp_server_3/usefile");
	while(1)
	{
		recv_fd(spfd,&new_fd);
		printf("child:new_fd=%d\n",new_fd);
		while(1)
		{
			//c存储的是客户端的命令
			memset(c,0,128);
			ret=recv(new_fd,c,128,0);
			printf("recv=%s\n",c);
			if(-1==ret)
			{	printf("\n"); }
			else if(0==ret)
			{
				close(new_fd);
				break;
			}
			command_handle(c,new_fd);
		}
		int flag=0;
		write(spfd,&flag,sizeof(flag));

	}
}

void make_child(pdata p,int num)
{
	int pid;
	int sv[2];
	
	for(int i=0;i<num;++i)
	{
		if(socketpair(AF_LOCAL,SOCK_STREAM,0,sv)==-1)
		{	perror("sockpair");exit(-1); }
		
		if(( pid=fork())==0)
		{
			close(sv[0]);
			child(sv[1]);
		}
		close(sv[1]);
		p[i].pid=pid;
		p[i].sfd=sv[0];
		p[i].busy=0;
	}
}


