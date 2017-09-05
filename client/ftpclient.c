 ///
 /// @file    ftpclient.c
 /// @author  wx(1418957094@qq.com)
 /// @date    2017-09-05 10:03:38
 ///

#include "include/head.h"

int main(int argc,char *argv[])
{
	if(argc!=3)
	{	printf("error args\n"); exit(-1); }

	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd==-1)
	{	perror("socket");exit(-1); }

	struct sockaddr_in seraddr;
	memset(&seraddr,0,sizeof(seraddr));
	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(atoi(argv[2]));
	seraddr.sin_addr.s_addr=inet_addr(argv[1]);

	if(connect(sfd,(struct sockaddr*)&seraddr,sizeof(seraddr))==-1)
	{	perror("connect"); close(sfd);exit(-1); }
	send(sfd,"hello",5,0);
	return 0;

}
