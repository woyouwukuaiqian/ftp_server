 /// /// @file    ftp.c
 /// @author  wx(1418957094@qq.com)
 /// @date    2017-09-05 09:02:25
 ///

#include "include/head.h"
#include "include/ftp.h"

int main()
{
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd==-1)
	{	perror("socket");exit(-1); }
	int pro_num=1;
	pdata p=(pdata)calloc(pro_num,sizeof(data_t));
	make_child(p,pro_num);

	struct sockaddr_in* ser = (sockaddr_in*)calloc(1,sizeof(sockaddr_in));
	ser->sin_addr.s_addr=INADDR_ANY;
	ser->sin_port=htons(8888);
	ser->sin_family=AF_INET;
	
	int ret = bind(sfd,(sockaddr*)ser,sizeof(sockaddr));
	if(ret==-1)
	{	perror("bind");close(sfd);exit(-1); }

	if(listen(sfd,10)==-1)
	{	perror("listen");close(sfd);exit(-1); }

	int epfd=epoll_create(1);
	if(-1==epfd)
	{	perror("epoll_create"); exit(-1) ; }
	
	struct epoll_event sock;
	memset(&sock,0,sizeof(sock));
	sock.events=EPOLLIN;
	sock.data.fd=sfd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&sock);

	struct epoll_event childEvent;
	for(int i=0;i<pro_num;++i)
	{
		memset(&childEvent,0,sizeof(childEvent));
		childEvent.events=EPOLLIN;
		childEvent.data.fd=p[i].sfd;
		epoll_ctl(epfd,EPOLL_CTL_ADD,p[i].sfd,&childEvent);
	}

	struct epoll_event* evs=(struct epoll_event*)calloc(pro_num+1,sizeof(epoll_event));
	int flag;
	while(1)
	{
		ret=epoll_wait(epfd,evs,pro_num+1,-1);
		for(int i=0;i<ret;++i)
		{
			if(evs[i].data.fd==sfd)
			{
				int new_fd=accept(sfd,NULL,NULL);
				printf("ftp:new_fd=%d\n",new_fd);
				if(new_fd==-1)
				{	perror("accept");exit(-1); }
				for(int j=0;j<pro_num;++j)
				{
					if(p[j].busy!=1)
					{
						send_fd(p[j].sfd,new_fd);
						close(new_fd);
						p[j].busy=1;
						break;
					}
				}
			}
			for(int j=0;j<pro_num;++j)
			{
				if(evs[i].data.fd==p[j].sfd)
				{
					read(p[j].sfd,&flag,sizeof(flag));
					printf("child is not busy\n");
					p[j].busy=0;
					break;
				}
			}
		}
	}

	wait(NULL);
	return 0;

}
