 ///
 /// @file    ftp.c
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

	int new_fd=accept(sfd,NULL,NULL);
	printf("new_fd=%d\n",new_fd);
	char c[10]={0};
	recv(new_fd,c,10,0);
	printf("c=%s\n",c);
	wait(NULL);
	return 0;

}
