 ///
 /// @file    ftpclient.c
 /// @author  wx(1418957094@qq.com)
 /// @date    2017-09-05 10:03:38
 ///

#include "include/head.h"
#include "include/ftp.h"


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
	
	int epfd=epoll_create(1);
	if(epfd==-1)
	{	perror("epoll_create");exit(-1); }
	struct epoll_event eventFd;
	memset(&eventFd,0,sizeof(eventFd));
	eventFd.events=EPOLLIN;
	eventFd.data.fd=sfd;
	if(epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&eventFd)==-1)
	{	perror("epoll_ctl");exit(-1); }
	
	struct epoll_event eventIO;
	memset(&eventIO,0,sizeof(eventIO));
	eventIO.events=EPOLLIN;
	eventIO.data.fd=0;
	if(epoll_ctl(epfd,EPOLL_CTL_ADD,0,&eventIO)==-1)
	{	perror("epoll_ctl");exit(-1); }

	struct epoll_event* evs=(struct epoll_event*)calloc(2,sizeof(eventFd));
	int commandFlag=1;
	int getsFlag=1;
	while(1)
	{
		memset(evs,0,sizeof(epoll_event)*2);
		int ret=epoll_wait(epfd,evs,2,-1);
//		printf("ret=%d\n",ret);
//		printf("fd=%d\n",evs[0].data.fd);
		for(int i=0;i<ret;++i)
		{
			if(evs[i].events==EPOLLIN && evs[i].data.fd==0)
			{
				if(commandFlag)
				{
					char buf[128]={0};
					read(0,buf,128);
					commandFlag=0;
					send(sfd,buf,strlen(buf)-1,0);
					commandFlag=1;
				}
				else
				{
					printf("ftpserver busy,try later\n");
					fflush(stdin);
				}

			}
			
			if(evs[i].data.fd==sfd && getsFlag)
			{
				printf("getsFlag=%d\n",getsFlag);
				char buf[MSGLEN]={0};
				recv(sfd,buf,sizeof(int)+MSGLEN,0);
				printf("sfd recv=%s\n",buf);
				command_handle(buf,&getsFlag,sfd);
			}
		}

	}
	return 0;

}
