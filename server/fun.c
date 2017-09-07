#include "include/ftp.h"


void func_cd(pcommand pmd,char* pbuf)
{
	char buf[128]={0};
	char path[128]={0};
	getcwd(buf,sizeof(buf));
	sprintf(path,"%s%c%s",buf,'/',pbuf);
	int ret=chdir(path);
	getcwd(path,sizeof(path));
	if(ret==-1)
	{
		strcpy(path,buf);
	}
	if(!strcmp("/home/wx/ftp_server_3/userfile",path))
	{
		strcpy(path,buf);
		chdir(path);
	}
	pmd->flag=1;
}

void func_ls(pcommand pmd)
{
	char path[128]={0};
	getcwd(path,sizeof(path));
	DIR *dir;
	dir=opendir(path);
	if(NULL==dir)
	{
		perror("opendir");
		return;
	}
	struct dirent *p;
	struct stat *buf=(struct stat*)calloc(1,sizeof(struct stat));
	char buf1[50]={0};
	char buf2[MSGLEN]={0};
	
	while((p=readdir(dir))!=NULL)
	{
		if(strcmp(p->d_name,".")&&strcmp(p->d_name,".."))
		{
			sprintf(buf1,"%s%s%s",path,"/",p->d_name);
			stat(buf1,buf);
			memset(buf1,0,sizeof(buf1));
			sprintf(buf1,"%d %10ld %-30s ",p->d_type,buf->st_size,p->d_name);
			sprintf(buf2,"%s%s%c",buf2,buf1,'\n');
		}
	}
	pmd->flag=0;
	strcpy(pmd->buf,buf2);
}

void func_puts(pcommand pmd,char* pbuf,int sfd)
{
	//告诉客服端要执行puts()
	command cmd;
	memset(&cmd,0,sizeof(cmd));
	cmd.flag=2;
	send(sfd,&cmd,sizeof(cmd),0);
	//发送文件名
	char file[128]={0};
	strcpy(file,pbuf);
	printf("%s\n",file);
	send(sfd,file,strlen(file),0);
	//检测文件是否合格
	int filesize=0;
	recv(sfd,&filesize,sizeof(int),0);
	printf("filesize=%d\n",filesize);	
	if(-1==filesize)
	{
		pmd->flag=-1;
		return ;
	}

	FILE* fp;
	int ret;
	int len=0;
	
	char buf[1000]={0};
	fp=fopen(pbuf,"a+");
	if(NULL==fp)
	{
		pmd->flag=-1;
		return ;
	}
	//断点续传
	fseek(fp,0,SEEK_END);
	int foffset=ftell(fp);
	send(sfd,&foffset,sizeof(foffset),0);

	int lensum=foffset;
	while(lensum<filesize)
	{
		ret=recv(sfd,&len,sizeof(int),0);
		printf("len=%d\n",len);
		if(0==ret)
		{
			fclose(fp);
			pmd->flag=-1;
			return ;
		}
		
		if(len>0)
		{
			lensum+=len;
			memset(buf,0,sizeof(buf));
			recv_n(sfd,buf,len);
			printf("%s\n\n\n",buf);
			fwrite(buf,len,1,fp);
		}else
		{
			break ;
		}
	}
	fclose(fp);
	pmd->flag=0;
	strcpy(pmd->buf,"success");
	printf("success\n");
}


void func_gets(ptrain pt,int sfd)
{
	
}

void func_remove(pcommand pmd,char* buf)
{
	if(remove(buf)==0)
	{
		pmd->flag=0;
		strcpy(pmd->buf,"success");
	}else
	{
		pmd->flag=-1;
	}
}

void my_pwd(pcommand pmd)
{
	getcwd(pmd->buf,MSGLEN);
	pmd->flag=0;
}	

