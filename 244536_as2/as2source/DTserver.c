#include "DTlib.h"
#define FIFO_PERMS (S_IRWXU | S_IWGRP| S_IWOTH)
#define BUFSIZE 128
#define LAGTIME 30
/*Server's listening fifo is COMMONFIFO defined in TDlib.h*/
pthread_mutex_t writemutex = PTHREAD_MUTEX_INITIALIZER;//use mutex to lock LOGFILE while writing, since multiple threads will log message to LOGFILE 
static FILE* logfile;

static int sigpipebufreceiver = 0;
static void sig_receiver(int signo){	
		char ret = signo;
		write(sigpipebufreceiver,&ret,1);
}

void checkSIGUSR2(int pipefdreaderside){
	for(;;){
		char mysignal;
		int res = read(pipefdreaderside,&mysignal,1);
		if (res == 1)
			break;
		else
			continue;
	}
}

/*The new thread save the pid of the client;opens the read end of the client-fifo(with the client pid its name);read the filename to be copied from the client-fifo and do the copy work */
void *thread_routine(void *arg){
	char* recbuf = strcpy(calloc((strlen(arg)+1),1),(char*)arg);	
	pid_t cpid = (pid_t)atoi(recbuf);
	//printf("THE CLIENT pid is to be opend is:%d\n", cpid);debugging sign	
	kill(cpid,SIGUSR1);//inform the client, which has just been dealt with, to release the semaphore so that new coming client can write it's request to the listening FIFO.check Line235!!
	/*this kill() ensures that the receive buffer is not modified by the main thread before the content is well saved,CHECK LINE 233!!*/
	kill(getpid(),SIGUSR2);//done copying the client request(essentially the pid of the new client), signal the main thread to go on listening.
	pthread_detach(pthread_self());
		

/*open client fifo as read only for data transfer*/
	char clientfifo[35];
	bzero(clientfifo,35);
	sprintf(clientfifo, "/tmp/DTfifo/%s", recbuf);
 	clientfifo[strlen(clientfifo)] = '\0';// uselessline, recbuf already have '\0'
	fprintf(logfile, "THE CLIENT FIFO to be opend is:%s\n", clientfifo);
	int clientfd;//clientfifo
	if((clientfd = open(clientfifo, O_RDONLY)) == -1){
		pthread_mutex_lock(&writemutex);
	       fprintf(logfile, "Failed to open client FIFO:%s\n", strerror(errno));
		fflush(logfile);//so that the logfile can be checked as soon as possible
		pthread_mutex_unlock(&writemutex);
		free(recbuf);
		return NULL;//pthread_exit(NULL); pthread_exit(NULL) create memory leak
	}
	pthread_mutex_lock(&writemutex);
	fprintf(logfile, "Clientfifo for process [%s] opened\n",recbuf);
	fflush(logfile);
	pthread_mutex_unlock(&writemutex);
/*Get the buffer length of the client fifo at run time and declare a buffer for reading client fifo*/	
	int pipelength = ((int)fpathconf(clientfd, _PC_PIPE_BUF)-5);// the DTclient instance would only write ((int)fpathconf(clientfd, _PC_PIPE_BUF)-10) each time
	pthread_mutex_lock(&writemutex);
	fprintf(logfile,"the client fifo buffer is [%d] bytes\n", pipelength);
	fflush(logfile);
	pthread_mutex_unlock(&writemutex);
  	char* DataTransferbuf = calloc(pipelength, 1);

/*first read from DT fd, the first read get the name of the file to be copied*/
	//kill(cpid,SIGUSR1);Why the hell is this line not working with client code line 114???!!!!!!!!!!!!!!!!!!!!!
	int nreadd = read(clientfd, DataTransferbuf, pipelength);//this line is dangerous, if one client goes down, this thread would block forever!!!!!!!!!!!!
	if (nreadd == 0)
	{
		fprintf(logfile,"invalid filename,thread terminate\n");
		free(DataTransferbuf);
		free(recbuf);
		close(clientfd);
		return NULL;//pthread_exit(NULL); pthread_exit(NULL) create memory leak
	}
	if(nreadd == -1)
	{
		fprintf(logfile,"read from clientfifo:%s\n", strerror(errno));
		free(DataTransferbuf);
		free(recbuf);
		close(clientfd);
		return NULL;//pthread_exit(NULL);
	}
/*save the filename and creat a new name with a 'COPY' suffix*/
	int k = strlen(DataTransferbuf);
	char* newname = NULL;
	newname = strcpy(malloc(k+5),DataTransferbuf);
	strcpy(&newname[k],"COPY");
/***********open file newnameCOPY for writing**********/
	FILE* filetowrite = NULL;
	if((filetowrite = fopen(newname, "w"))==NULL){
		perror("fopen");
		free(newname);
		free(DataTransferbuf);	
		free(recbuf);
		close(clientfd);
		return NULL;//pthread_exit(NULL); 
	}
	pthread_mutex_lock(&writemutex);
	fprintf(logfile,"%s opened for saving the client file\n",newname);
	fflush(logfile);
	pthread_mutex_unlock(&writemutex);
/*now prepared to receive data, send SIGUSR1 to initiate the client transfer*/
	kill(cpid,SIGUSR1);//go-signal from the server, indicating the client to transfer the file content
	while ((nreadd = read(clientfd, DataTransferbuf, pipelength))>0){
		fwrite(DataTransferbuf, 1, nreadd, filetowrite);
		fflush(filetowrite);//debuging
		kill(cpid,SIGUSR1);
	}
	pthread_mutex_lock(&writemutex);
	fprintf(logfile,"%s successfully created! Thread terminated!\n",newname);
	fflush(logfile);
	pthread_mutex_unlock(&writemutex);	
/*cleaning up, also try to unlink useless fifos and directory*/
	close(clientfd);
	if(unlink(clientfifo)!=0){
		fprintf(logfile, "unlink clientfifo: %s\n", strerror(errno));
	}
	free(newname);
	free(DataTransferbuf);	
	free(recbuf);
	fclose(filetowrite);	
	return NULL;//pthread_exit(NULL);
}
		

int main(int argc, char* argv[])
{
	if (argc!=2){
		printf("\nPlease give an argument to set a timeout for the daemon server.\nUsage: ./DTserver [timeout_in_minute]\n\n");
		return 0;
	}
	//turn into daemon
	if (turn_into_daemon() < 0) {
		perror("daemon");
		exit(2);
	}
	
/*set pipe to buffer SIGUSR1 and sigaction to deal with SIGUSR1*/
	int pipefdreceiver[2];
	assert(pipe(pipefdreceiver) == 0);
	sigpipebufreceiver = pipefdreceiver[1];	
/*use sigaction to install handler for SIGUSR2 CHECK LINE29 AND LINE 194 TO SEE THE FUNCTION OF THIS HANDLER*/	
	struct sigaction sigreceiver;
	sigemptyset(&sigreceiver.sa_mask);
	sigreceiver.sa_flags = 0;
	sigreceiver.sa_handler = sig_receiver;
	assert((sigaction(SIGUSR2,&sigreceiver,NULL))==0);
	
/*Request a semaphore, the semaphore is used to grant a client the writing permission to the listeing-fifo*/
	sem_t *id;
	assert((id = sem_open(SEMA,O_CREAT,0777,1)) != SEM_FAILED);
	printf("Successfully allocated semaphore id %p\n", (void *)id);
	
/*open LOGFILE for logging*/
	logfile = NULL;
	if((logfile = fopen("LOGFILE","a")) == NULL){
		perror("fopen");
		return 1;
	}
/*create a directory for all FIFOS so that clean up of FIFOS can be easier*/	
	if((mkdir(FIFODIR, 0777) == -1) && (errno != EEXIST))//FIFODIR defined in DTlib.h
	{
		fprintf(logfile, "Try create a directory for fifos:%s\n", strerror(errno));
		return 1;
	}

/*create a well-known FIFO for listening to request */	
	if ((mkfifo(COMMONFIFO, FIFO_PERMS) == -1) && (errno != EEXIST))
	{
		fprintf(logfile, "Failed to create a FIFO:%s\n", strerror(errno));
		return 1;
	}

	int listenfd;
	if ((listenfd = open(COMMONFIFO, O_RDWR)) == -1) //O_RDWR;open as read write to prevent encounter end of file and shut off the pipe end prematurely.
        {
          	perror("Failed to open FIFO");
		fprintf(logfile, "Failed to open FIFO:%s\n", strerror(errno));
          	return 1;
        }
/*Use a select() call to listen to the listening fifo, also acts as a timer for the server!*/
	int nread;
	char recbuf[BUFSIZE];
	bzero(recbuf,BUFSIZE);//////////////////////NEWLY ADDED
//initialization for the select call
	int timeout = 60*atoi(argv[1]);//atoi shall be called, type conversion don't work
	int maxfd=listenfd+1;
	struct timeval tv;
	fd_set readfds;
	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	int returnV=0;
	FD_ZERO(&readfds);
	FD_SET(listenfd, &readfds);
	fprintf(logfile, "Timeout is set to be %d seconds\n", timeout);
	fflush(logfile);//so that I can check the log as soon as the server start
//use select as a timer to timeout the daemon server.
	while((returnV = select(maxfd, &readfds, NULL, NULL, &tv))>=0)
	{
		if(returnV == -1){
			pthread_mutex_lock(&writemutex);
			fprintf(logfile, "select():%s\n",strerror(errno));
			pthread_mutex_unlock(&writemutex);
			break;//go to clean up steps
		}
		if(returnV == 0){
			pthread_mutex_lock(&writemutex);
			fprintf(logfile, "server listening-time up!Server shutting down in 30 seconds.\n");
			pthread_mutex_unlock(&writemutex);
			//when shuting down, only pthread_exit(NULL) called, making sure the on-running threads can finish their jobs
			break;//go to clean up steps
		}
		if(returnV)/*if(FD_ISSET(listenfd,&readfds)) works alternatively*/
		{
			/*no need to call FD_ISSET since there's only one fd in the set*/
			pthread_mutex_lock(&writemutex);
			fprintf(logfile,"new request catched!");
			fflush(logfile);
			pthread_mutex_unlock(&writemutex);
			bzero(recbuf,BUFSIZE);
			nread = read(listenfd, recbuf, BUFSIZE);
			//read more than needed caused the problem!!.BUFSIZE used to be 5(when pid is not 4 digits,then things go wrong)
			//This problem is solved by adding line 236, 237 in DTserver.c and line 107 in DTclient.c - client request synchronization
			if(nread == 0)
			continue;
			if (nread>0){
				pthread_mutex_lock(&writemutex);			
				fprintf(logfile, "The client pid is : %s\n", recbuf);
				fflush(logfile);
				pthread_mutex_unlock(&writemutex);
				pthread_t thread_id;
				assert((pthread_create(&thread_id,NULL,thread_routine,(void*)recbuf)) == 0);//call the thread routine to perform per-thread based data transfer
				checkSIGUSR2(pipefdreceiver[0]);//before the new thread copied the content of the recbuf, the main thread shall hangs here, other wise, the content of recbuf may be modified by the main thread loop before it's used by the newly created thread!!CHECK LINE 33

/*the following two lines are replace in the thread routine, line31! This change prevent the case when main thread timeout and break from the listening task, client wait for SIGUSR1 and blocks forever before releasing semaphore. In that case, that client dangels forever and semaphore is never returned*/
				//pid_t oldclientpid = (pid_t)atoi(recbuf);
				//kill(oldclientpid,SIGUSR1);//inform the client, which has just been dealt with, to release the semaphore so that new coming client can write it's request to the listening FIFO.
						
			}			
				
		}
			
	}
	close(listenfd);//no longer accept new coming request, client would get SIGPIPE and be terminated
	sleep(LAGTIME);// wait for the last servered client to release its semaphore and wait for working threads to finish their jobs
/*try to unlink the fifo, if child thread still running, then I have to manually unlink the trash, but it's within one folder, easy*/
	if (unlink(COMMONFIFO)!=0){
		pthread_mutex_lock(&writemutex);
		fprintf(logfile, "unlink COMMONFIFO: %s\n", strerror(errno));
		pthread_mutex_unlock(&writemutex);
	}//client specific fifos are unlinked by the DTclient instances when they return so that rmdir could be successful
	if (rmdir(FIFODIR)!=0){
		pthread_mutex_lock(&writemutex);
		fprintf(logfile, "rmdir: %s\n", strerror(errno));
		pthread_mutex_unlock(&writemutex);
	}	
	sem_close(id);//closing the sema is ok as the server is already not issuing anymore new thread.(semaphore is released)
	sem_unlink(SEMA);//semaphore is defined in DTlib.h and used by DTclient
	fclose(logfile);
	pthread_mutex_destroy(&writemutex);
	pthread_exit(NULL);
}


					
		

	
		
	
	
	
	
		
