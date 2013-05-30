#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include "morse.h"
#include <signal.h>
#include <unistd.h>
#include <assert.h>

#define BUFLEN 1024

/***define signal handler so that signals are buffered and never lost***/
static int sigpipebufsender = 0;
static void sig_sender(int signo){
		char rett = signo;
		write(sigpipebufsender,&rett,1);
}	

/******************Sender task that open a file and read characters into memory buffer, call the sendcode to send signals for a certain character************************/
void senderprocess(char* inputfile, int partypid, int pipefdreaderside){
/**********open file for logging***************/
	FILE* logfile = NULL;//don't have to use filelock since two processes access the file in order,just fflush after each fprintf()
	if((logfile = fopen("LOGFILE","a")) == NULL){
		perror("fopen");
		exit(EXIT_FAILURE);
	}
/*********open the file to read**************/
	FILE* filetoread = NULL;
	if((filetoread = fopen(inputfile, "r"))==NULL){
		perror("fopen");
		printf("file do not exist!\n");
		kill(partypid,SIGTERM);//notify the receiver to close file!
		exit(EXIT_FAILURE); 
	}
	fprintf(logfile,"%s sucessfully opened at sender side!\n",inputfile);
	fflush(logfile);//flush the underlaying buffer every time in order for writting the log to file in time order.
	
	char redbuf[BUFLEN];
	bzero(&redbuf,BUFLEN);
	int inbuf = 0;

	int i=0;//index of the buffer
	while((inbuf = fread(&redbuf, 1,BUFLEN, filetoread))!=0){
		for(i=0; i<inbuf; i++){
			fprintf(logfile,"Parent: The sender is waiting for 'Go' signal for sending the MorseCode Sequence of the next character\n");//the 'Go' is SIGINT
			fflush(logfile);
			checkstate(pipefdreaderside);//blocking until a 'Go' is caught
			fprintf(logfile,"Parent: 'Go' received; The character to be sent is '%c',calling sendcode() to send the corresponding morsecode sequence\n",redbuf[i]);
			fflush(logfile);
			sendcode(partypid,pipefdreaderside,redbuf[i]);//send the morse code for the specific character
			fprintf(logfile,"Parent: successfully sent morse code sequence for character '%c'\n",redbuf[i]);
			fflush(logfile);
		/********character diving after every sendcode return,telling the decoder to parse the sequence and write to file*************/
			checkstate(pipefdreaderside);
			kill(partypid,SIGALRM);//send SIGALRM for character dividing,sendsignal()doesnt' define SIGALRM, so use kill here
			fprintf(logfile,"Parent: Character_dividing signal sent\n");
			fflush(logfile);
		}
		//i = 0;//unecessary line
	}
	fclose(filetoread);
	kill(partypid,SIGTERM);//send SIGTERM,telling the receiver to close the file
	fprintf(logfile,"Parent: SIGTERM SENT,CHILD SHALL CLOSE ALL FILES AND EXIT\n");
	fclose(logfile);// both process must close the file, the file handlers are process independent.
	return;
}


int main(int argc, char* argv[])
{
	if (argc!=2){
		printf("Type the name of the file to be opened as a command-line argument\n");
		exit(EXIT_FAILURE);
	}
/********blocking useless signal so that the transfer will not be interrupted by unwanted signals, could be useful; Tested with no problem*******/
/*	sigset_t newmask;
	sigfillset(&newmask);
	sigdelset(&newmask,SIGINT);
	sigdelset(&newmask,SIGUSR1);
	sigdelset(&newmask,SIGUSR2);
	sigdelset(&newmask,SIGALRM);
	sigdelset(&newmask,SIGTERM);
	sigdelset(&newmask,SIGCONT);
	sigprocmask(SIG_BLOCK,&newmask,NULL);
*/	
/*************set pipebuffer for both child and parent and set the corresponding signal handlers for both processes****************/
	int pipefdsender[2];
	
	assert(pipe(pipefdsender) == 0);
	sigpipebufsender = pipefdsender[1];//set for parent the sender to use
	
	struct sigaction sigsender;//deal with "go" signal(SIGINT) from the receiver(child)
	sigemptyset(&sigsender.sa_mask);
	sigsender.sa_flags = 0;
	sigsender.sa_handler = sig_sender;//the handler name is sig_sender
	assert((sigaction(SIGINT,&sigsender,NULL))==0);
	
	char *argv_execv[]={"decoder", argv[1], NULL};// load the arguments pointer for exec() code.
/***************fork the process*****************/
	pid_t tpid;
	tpid = fork();
/**************the Parent process also the sender*******************/	
	if(tpid>0){
		senderprocess(argv[1],tpid,pipefdsender[0]);					
	}
/***************child process also the decoder************************/	
	if(tpid==0){
		close(pipefdsender[0]);
		close(pipefdsender[1]);
		pid_t ppid = getppid();
		/*********swap code**********/
		//char *argv_execv[]={"decoder", argv[1], NULL};//encountered warning,Do this initialization before fork() can slove the problem.
		if(execv("decoder",argv_execv)<0){
			perror("Error on execv");	
			kill(ppid,SIGKILL);//kill the parent,otherwise the parent would hang there waiting for 'Go' forever
		}
	}
	return 0;
}


