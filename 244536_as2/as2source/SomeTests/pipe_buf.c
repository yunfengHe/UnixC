#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
	int length;
	mkfifo("COMMONFIFO", S_IRWXU | S_IWGRP| S_IWOTH);
	int listenfd = open("COMMONFIFO", O_RDWR);
	length = (int)fpathconf(listenfd, _PC_PIPE_BUF);//PIPE_BUF,_PC_PIPE_BUF,_POSIX_PIPE_BUF
//	int new = _POSIX_PIPE_BUF;
	printf("pipe_buf is %d bytes\n",length);
//	printf("the value of the macro is %d\n",new);
	return 0;
}

