#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "head.h"
#define FILENAME "file"
#define MSGLEN 1000

typedef struct {
	int pid;
	int sfd;
	int busy;//子进程为0不忙绿，�?忙碌
}data_t,*pdata;

typedef struct {
	int len;
	char buf[1000];
}train,*ptrain;

typedef struct {
	int flag;
	char buf[MSGLEN];
}command,*pcommand;

int command_handle(char* buf,int new_fd);
void send_fd(int,int);
void recv_fd(int,int*);
int send_n(int,char*,int);
void make_child(pdata,int);
int recv_n(int,char*,int);
void func_cd(pcommand,char* path);
void func_ls(pcommand);
void func_puts(pcommand,char* path,int new_fd);
void func_gets(pcommand,char* path,int new_fd);
void func_remove(pcommand,char* path);
void my_pwd(pcommand);
#endif
