#include "DTlib.h"
#define BUFLEN 1024
#define FIFO_PERMSCLIENT (S_IRWXU | S_IRWXG| S_IRWXO)

static int sigpipebufreceiver = 0;
static void sig_receiver(int signo){	
		char ret = signo;
		write(sigpipebufreceiver,&ret,1);
}

void checkSIGUSR1(int pipefdreaderside){
	for(;;){
		char mysignal;		
		int res = read(pipefdreaderside,&mysignal,1);
		if (res == 1)
			break;
		else
			continue;
	}
}

void writefile(char* inputfile, int DTfd, int pipefdreaderside)
{
/*get the length of the pipe buffer; 
calling fpathconf at run time make sure it's always right*/	
	int pipelength = ((int)fpathconf(DTfd, _PC_PIPE_BUF)-10);//PIPE_BUF,_PC_PIPE_BUF
/*********open the file to read**************/
	FILE* filetoread = NULL;
	if((filetoread = fopen(inputfile, "r"))==NULL){
		perror("fopen");
		printf("file do not exist!\n");
		close(DTfd);//is this really useful? shall I use the content length method??
		exit(EXIT_FAILURE); //do I have to register atexit? I don't think so
	}
	printf("%s sucessfully opened at sender side!\n",inputfile);
/*read from file and write to DataTransfer FIFO*/
	char *redbuf= calloc(pipelength, 1);
	int inbuf = 0;

	checkSIGUSR1(pipefdreaderside);
	while((inbuf = fread(redbuf, 1,pipelength, filetoread))!=0){
		write(DTfd,redbuf,inbuf);
		/*check for SIGUSR1 to send more data*/
		checkSIGUSR1(pipefdreaderside);		
	}
	free(redbuf);
	fclose(filetoread);
	close(DTfd);
	printf("client[%d] done writing, terminated\n",getpid());
	return;
}


int main(int argc, char* argv[])
{

	if (argc != 2){
		printf("\nUsage: ./DTclient [filename] \n");//argv[1] == filename
		return 0;
	}
/*set pipe to buffer SIGUSR1 and sigaction to deal with SIGUSR1*/
	int pipefdreceiver[2];
	assert(pipe(pipefdreceiver) == 0);
	sigpipebufreceiver = pipefdreceiver[1];	
	
	struct sigaction sigreceiver;
	sigemptyset(&sigreceiver.sa_mask);
	sigreceiver.sa_flags = 0;
	sigreceiver.sa_handler = sig_receiver;
	assert((sigaction(SIGUSR1,&sigreceiver,NULL))==0);
/*Open up the existing semaphore*/
	sem_t *id;
	assert((id = sem_open(SEMA,0)) != SEM_FAILED);
	printf("Using existing semaphore %p.\n", (void *)id);

/*save pid to a buffer for later use*/
	char buf[25];
	bzero(buf, 25);
	pid_t mypid = getpid();
	sprintf(buf,"%d",mypid);	
	buf[strlen(buf)] = '\0';

/*make a clientfifo before sending any request to the server*/
	char clientfifoname[35];
	bzero(clientfifoname, 35);
	sprintf(clientfifoname,"/tmp/DTfifo/%s",buf);//"/tmp/%sfifo"
	//clientfifoname[strlen(clientfifoname)] = '\0'; 
	
	if ((mkfifo(clientfifoname, FIFO_PERMSCLIENT) == -1) && (errno != EEXIST))//clientfifoname CLIENTFIFO1
	{
		perror("Failed to create a FIFO");
		printf("Failed to create a FIFO\n");
		return 1;
	}
	printf("client fifo created!\n");

/*send request to the server by writing the client pid to the listening FIFO*/
	int listenfd;
	if((listenfd = open(COMMONFIFO,O_WRONLY,O_APPEND)) == -1)
	{
		perror("failed to open fifo on client side\n");
		return 1;	
	}
	//requestsem(id);//before sending the request to server, wait for semaphore to access the listening fifo.//dangerous
	if(trysem(id) == -1){sem_close(id); printf("cannot get semaphore within 10 seconds,client shut down\n"); return 1;}	//trysem() defined in DTlib.h		
	write(listenfd,buf,(strlen(buf)+1));//(strlen(buf)+1) would include the '\0'
	checkSIGUSR1(pipefdreceiver[0]);//before server send SIGUSR1,do not release semaphore,this ensures only one client write to the listening fifo before server could read//dangerous blocking!!!!!!! But already solved by modifying the server code, check comment line 235 in DTserver.c
	releasesem(id);//after sending request, release semaphore
	printf("request containing pid sent!\n");
	close(listenfd);//JUST MODIFIED	
	
/*open the write end of the DT fifo must happen after read end opend, otherwise, block*/
	//sleep(2);//checkSIGUSR1(pipefdreceiver[0]);//these are possible ways to deal with the block
	//checkSIGUSR1(pipefdreceiver[0]);This line don't work with DTserver.c line 63!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	int DTfd;	
	if((DTfd = open(clientfifoname,O_RDWR)) == -1)//O_RDWR can solve the block problem, O_WRONLY would encounter SIGPIPE???
	{
		perror("failed to open fifo on client side\n");
		return 1;	
	}
	printf("client fifo write end opened!\n");

/*write to the DataTransfer fifo first time, sending the name of the file to be copied*/

	write(DTfd,argv[1],(strlen(argv[1])+1));	

/*now waiting for signal and write to fifo*/
	writefile(argv[1],DTfd,pipefdreceiver[0]);
	close(DTfd);
	sem_close(id);//JUST MODIFIED
	//unlink(clientfifoname); leave these work to the server thread routine. it's the reader's job to unlink the fifo and close sem!!
	return 0;
}

		
	
	






