 ///
 /// @file    child.c
 /// @author  wx(1418957094@qq.com)
 /// @date    2017-09-05 12:35:43
 ///

#include "include/head.h"
#include "include/ftp.h"

void child(int spfd)
{
	while(1)
	{
		printf("pid=%d\n",getpid());
		::sleep(2);
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


