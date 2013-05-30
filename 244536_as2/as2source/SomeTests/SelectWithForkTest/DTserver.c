#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/select.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

#define COMMONFIFO "/tmp/newfifo"
#define TIMELIMIT 5 //the daemon will shut off 30 minutes later after launching
#define FIFO_PERMS (S_IRWXU | S_IWGRP| S_IWOTH)
#define BUFSIZE 128


/*the listening fifo is COMMONFIFO defined in TDlib.c*/

//acceptDT(char* buf);
//struct linkedlist();

int main()
{
	int listenfd;
	char recbuf[BUFSIZE];
	pid_t childpid;
	int nread;

	/*open LOGFILE for logging*/
	FILE* logfile = NULL;
	if((logfile = fopen("LOGFILE","a")) == NULL){
		perror("fopen");
	}
	

	if ((mkfifo(COMMONFIFO, FIFO_PERMS) == -1) && (errno != EEXIST))
	{
		fprintf(logfile, "Failed to create a FIFO:%s\n", strerror(errno));
		return 1;
	}


	childpid = fork();
	
	if(childpid == -1)
	{
		fprintf(logfile, "Failed to fork:%s\n", strerror(errno));
		return 1;
	}

	if(childpid){
		if ((listenfd = open(COMMONFIFO, O_RDWR)) == -1) /*O_RDWR| O_NONBLOCK*///open as read write to prevent encounter end of file and shut off the pipe end prematurely.
        	{
          		perror("Failed to open FIFO");
			fprintf(logfile, "Failed to open FIFO:%s\n", strerror(errno));
          		return 1;
        	}
//initialization for the select call
		int maxfd=listenfd+1;
		struct timeval tv;
		fd_set readfds;
		//fd_set writefds;
		tv.tv_sec = TIMELIMIT;
		tv.tv_usec = 0;
		int returnV=0;
		FD_ZERO(&readfds);
		FD_SET(listenfd, &readfds);
//use select as a timer to timeout the daemon server.
		while((returnV = select(maxfd, &readfds, NULL, NULL, &tv))>=0)
		{
			if(returnV == -1){
				fprintf(logfile, "select():%s\n",strerror(errno));
				exit(EXIT_FAILURE);
			}
			if(returnV == 0){
				fprintf(logfile, "server time up!\n");
				break;
			}
			if(returnV)/*or if(FD_ISSET(listenfd,&readfds))*/
			{
				fprintf(logfile,"new request catched!\n");
				printf("new request catched!\n");
				/*no need to call FD_ISSET since there's only one fd in the set*/
				nread = read(listenfd, recbuf, 6);/*BUFSIZE*/
				if(nread == 0)
				continue;
				if (nread>0){
					//recbuf[nread] = '\0';
					printf("server got the client pid %s\n", recbuf);
					
				}
				
				//do something;
			
				//acceptDT(&recbuf);
			}
			
		}
	}



	else{
		fork();
		fork();

		if( (listenfd = open(COMMONFIFO,O_WRONLY)) == -1)
		{
			perror("failed to open fifo on client side\n");
			return 1;	
		}
		
		char buf[BUFSIZE];//BUFSIZE LONG IN TOTAL
		bzero(buf, BUFSIZE);
		int buflen;

		pid_t tmp = getpid();
		printf("I am the child and the pid is %d\n", tmp);
		//fprintf might be easier!!
		sprintf(buf,"%d",tmp);
		buflen = strlen(buf);
		buf[buflen] = '\0';
		write(listenfd,buf,buflen+1);
		close(listenfd);
	}
	close(listenfd);
	return 0;
}

			
		

	
		
	
	
	
	
		
