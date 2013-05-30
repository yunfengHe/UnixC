#ifndef LIBSER_H
#define LIBSER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/select.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define SEMA "/foo-bar"
#define FIFODIR "/tmp/DTfifo"
#define COMMONFIFO "/tmp/DTfifo/newlistenfifo"
#define WORKINGDIR "/tmp/DTserver"
#define TRYSEMTIMEOUT 10

static int semheld = 0;
void releasesem(sem_t *id);
void requestsem(sem_t *id);
int trysem(sem_t *id);
void getsemvalue(sem_t *id);
int turn_into_daemon(void);
void closeall(void);
void getlock(int fd, int type);


void releasesem(sem_t *id) {
  if (semheld < 1) {
    printf("I don't have any resources; nothing to release.\n");
    return;
  }
  sem_post(id);
  semheld--;
  printf("Resource released.\n");
}

void requestsem(sem_t *id) {
  if (semheld > 0) {
    printf("I already hold the resource; not requesting another one.\n");
    return;
  }

  printf("Requesting resource...");
  fflush(stdout);

  sem_wait(id);
  semheld++;

  printf(" done.\n");
}

int trysem(sem_t *id)
{
		
 	if (semheld > 0) {
    		printf("I already hold the resource; not requesting another one.\n");
    		return 0;
  	}
  	printf("Trying to request resource...");
  	fflush(stdout);
/*
	struct timespec timeout;
	timeout.tv_sec = TRYSEMTIMEOUT;
	timeout.tv_nsec = 0;
	if(sem_timedwait(id, &timeout) == -1)// read carefully of the manual. The  abs_timeout  argument  points  to a structure that specifies an absolute timeout in seconds and nanoseconds since the Epoch (00:00:00, 1 January 1970). This is so shit!!!!!
	{
		perror("Semphore not acquired within timeout");
		return -1;
	}
*/
	time_t start = time(NULL);
	time_t current;
	while(1){
		if(sem_trywait(id) == 0){
			semheld++;
  			printf(" done.\n");
			return 0;
		}
		//sleep(1);
		current = time(NULL);
		if ((current - start)>10)
		break;
	}
	return -1;
}

void getsemvalue(sem_t *id)
{
	int sval;
	if(sem_getvalue(id,&sval) == -1)
	{
		perror("sem_getvalue");
	}
	else
	{
		if(sval > 0)
			printf("Semaphore value %d\n",sval);
		else if (sval == 0)
			printf("Semaphore locked, no processes waiting\n");
		else
			printf("Semaphore locked, %d processes waiting\n",sval*-1);
	}
}

int turn_into_daemon(void)
{
	// Fork again to sever ties to parent.
	switch (fork())  {
		case 0:  break;     /* We're the child, ok. */
		case -1: return -1; /* Fork failed! */
		default: _exit(0);  /* Parent exits. */
	}

	printf("My pid %d, my ppid %d, my gpid %d\n",getpid(),getppid(),getpgrp());

	// Get a new session.
	assert(setsid() > 0);               /* shouldn't fail */

	/* Comment out this switch if you want to acquire a control tty in
	 * the future -- not normally advisable for daemons. */
	switch (fork()) {
		case 0:  break;
		case -1: return -1;
 		default: _exit(0);
	}

	printf("My pid %d, my ppid %d, my gpid %d\n",getpid(),getppid(),getpgrp());
	closeall();
	open("/dev/null",O_RDWR);
	dup(0);  /* Copy /dev/null also as stdout stream */
	dup(0);  /* Copy /dev/null also as stderr stream */
	
	
	umask(0);//clear file mode creation mask

	/* Get to the working directory */
	if((mkdir(WORKINGDIR, 0777) == -1) && (errno != EEXIST))
	{
		perror("Try create a directory for fifos:");
		return -1;
	}
	chdir(WORKINGDIR);	

	return 0;
}

void closeall(void) {
	int fd = 0;
	int fdlimit = sysconf(_SC_OPEN_MAX);
	while (fd < fdlimit)
		close(fd++);
}

/*I planned to use this function to protect the newly created COPY files. but I used fopen(,"w") in my server, then it's not needed*/
void getlock(int fd, int type) {
	struct flock lockinfo;    
    /* we'll lock the entire file */
	lockinfo.l_whence = SEEK_SET;
	lockinfo.l_start = 0;
	lockinfo.l_len = 0;
	lockinfo.l_type = type; //F_WRLCK; F_UNLCK  
   /*Wait to get the lock*/
	while (1){
		if(fcntl(fd, F_SETLKW, &lockinfo) == -1){
			perror("Gelock");// possibly interrupt
			continue;
		}
		else
		break;
	}
/*when a fd is closed, the writelock will be automatically released!! no need to use F_UNLCK really*/
}



#endif




