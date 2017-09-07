 ///
 /// @file    fun.c
 /// @author  wx(1418957094@qq.com)
 /// @date    2017-09-05 22:50:15
 ///

#include "include/ftp.h"
#include <sys/stat.h>

//puts()
int func2(int sfd){
	char filename[128]={0};
	recv(sfd,filename,sizeof(filename),0);
	printf("filename=%s\n",filename);	
	int fd;
	int filesize=0;
	fd=open(filename,O_RDONLY);
	if(-1==fd)
	{
		filesize=-1;
		send(sfd,&filesize,sizeof(filesize),0);
		printf("open file fail\n");
		close(fd);
		return -1;
	}
	struct stat fbuf;
	memset(&fbuf,0,sizeof(fbuf));
	fstat(fd,&fbuf);
	filesize=fbuf.st_size;
	send(sfd,&filesize,sizeof(filesize),0);
	//断点续传
	int foffset=0;
	recv(sfd,&foffset,sizeof(foffset),0);
	lseek(fd,foffset,SEEK_SET);

	train t;
	int sizeRemain=filesize-foffset;
//	if(sizeRemain<200*(1<<20))
	if(sizeRemain<200*(1<<2))//判断传输大小比标准值大时用mmap
	{
		while(1)
		{
			memset(&t,0,sizeof(t));
			t.len=read(fd,t.buf,sizeof(t.buf));
			if(t.len<=0)
			{
				close(fd);
				return 0;
			}
			printf("func2 len=%d\n",t.len);
			printf("func2 buf=%s\n",t.buf);
			if(-1==send_n(sfd,(char*)&t,4+t.len))
			{
				close(fd);
				return -1;
			}
			
		}
	}else
	//mmap映射
	{
		void* start=NULL;
		void* pfile=NULL;
		int lensum=0; 
		int lenOne=900;
		pfile=mmap(start,sizeRemain,PROT_READ,MAP_PRIVATE,fd,foffset);
		printf("mmap\n");
		char* putsp=(char*)pfile;
		printf("mmap1\n");
		while(lensum<sizeRemain)
		{
			memset(&t,0,sizeof(t));
			lenOne=((sizeRemain-lensum)<900?(sizeRemain-lensum):900);
			t.len=lenOne;
		printf("mmap2\n");
			strncpy(t.buf,putsp,lenOne);
		printf("mmap3\n");
			if(-1==send_n(sfd,(char*)&t,4+t.len))
			{
				close(fd);
				return -1;
			}
			putsp+=lenOne;
			lensum+=lenOne;
		}
	}
	close(fd);
	return 0;
}
//gets()
void func3(int sfd){}


int command_handle(char* buf,int* pflag,int sfd)
{
	pcommand pcd=(pcommand)buf;
//	printf("pflag=%d\n",pcd->flag);
	switch(pcd->flag)
	{
		case 0: printf("%s\n",pcd->buf);break;
		case 1: break;
		case 2: *pflag=0;if(-1==func2(sfd))printf("puts fail\n");*pflag=1;break;//puts
		case 3: *pflag=0,func3(sfd);*pflag=1;break;//gets
		case -1: printf("command failed\n");return -1;
	}
	return 0;
}

int command_check(char* buf)
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
	if(!strcmp(buf1,"puts"))
	{
		int fd=open(buf1,O_RDONLY);
		if(-1==fd)
		{	
			close(fd);
			perror("open");
			return -1;
		}
		close(fd);
		return 2;
	}
	else if(!strcmp(buf2,"gets"))
	{
		return 3;
	}
	return 0;
}
