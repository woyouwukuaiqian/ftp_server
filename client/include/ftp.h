#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "head.h"
#define FILENAME "file"
#define MSGLEN 1000

typedef struct {
	int pid;
	int sfd;
	int busy;
}data_t,*pdata;

typedef struct {
	int len;
	char buf[1000];
}train,*ptrain;

typedef struct {
	int flag;
	char buf[MSGLEN];
}command,*pcommand;

int command_handle(char* buf,int* flag,int sfd);
void send_fd(int,int);
void recv_fd(int,int*);
int send_n(int,char*,int);
void make_child(pdata,int);
int recv_n(int,char*,int);
void func_cd(ptrain,int);
void func_ls(ptrain,int);
void func_puts(ptrain,int);
void func_gets(ptrain,int);
void file_delete(ptrain,int);
void my_pwd(int,int);
#endif
