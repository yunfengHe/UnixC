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

static int sigpipebufreceiver = 0;
static void sig_receiver(int signo){	
		char ret = signo;
		write(sigpipebufreceiver,&ret,1);
}


/******************Sender task: open a file and read characters into memory buffer; call sendcode() to send morse code sequence************************/

void senderprocess(char* inputfile, int partypid, int pipefdreaderside){
	/**********open LOGFILE for logging***************/
	FILE* logfile = NULL;//don't have to use filelock since two processes access the file in order(ONE SIGNAL AT A TIME),just fflush after each fprintf()
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
		exit(EXIT_FAILURE); //do I have to register atexit? I don't think so
	}
	fprintf(logfile,"%s sucessfully opened at sender side!\n",inputfile);
	fflush(logfile);//flush the underlaying buffer every time,in order for writting the log to file in time order.
	
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
		/********character diving after every sendcode() return,telling the decoder to parse the sequence and write to file*************/
			checkstate(pipefdreaderside);
			kill(partypid,SIGALRM);//send SIGALRM for character dividing,my sendsignal()doesnt' define SIGALRM, so use kill here
			fprintf(logfile,"Parent: Character_dividing signal sent\n");
			fflush(logfile);
		}
		//i = 0;//unecessary line
	}
	fclose(filetoread);//close the file also cause the fopen internal memory allocation be freed, when I forgot to close, valgrind shows memory leak!!!
	kill(partypid,SIGTERM);//send SIGTERM,telling the receiver to close the file
	fprintf(logfile,"Parent: SIGTERM SENT,CHILD SHALL CLOSE ALL FILES AND EXIT\n");
	fclose(logfile);//each process holds a handler of the file, it doesn't matter if the file is closed here, won't affect the child process
	return;
}


/**************Receiver task that giving 'go' signal to the sender and wait to receive signal sets for certain characters, parse it, write to file********************/

void decoderprocess(char *filename,int senderpid, int pipefdreaderside){
/**********open file for logging***************/
	FILE* logfile = NULL;
	if((logfile = fopen("LOGFILE","a")) == NULL){
		perror("fopen");
		exit(EXIT_FAILURE);
	}
/*********a new name for the copy of the file**********/
		int k = strlen(filename);
		char* newname = NULL;
		newname = strcpy(malloc(k+5),filename);
		strcpy(&newname[k],"COPY");
/***********open a file for writing**********/
		FILE* filetowrite = NULL;
		if((filetowrite = fopen(newname, "w"))==NULL){
			perror("fopen");
			free(newname);
			exit(EXIT_FAILURE); 
		}
		free(newname);
/************give 'go' code to the sender and receive morse code, parse the code and save charecters in file**************/
		char morse = 0x01;/*A MORSE CODE SEQUENCE HOLDER;use bitwise operation to hold the morse digits within one byte; The first bit MUST BE 1 so that info don't get lost!!*/
		char mysignal;//used to contain the received signal		
		for(;;){
			kill(senderpid,SIGINT);//the 'Go' signal - send to child a SIGINT to indicate ready-to-receive;the receiver is the initializer of the transfer process.
			//printf("Child: 'Go' sent to the sender to indicate ready-to-receive\n");//debugging sign
			for(;;){
				int res = read(pipefdreaderside,&mysignal,1);
				if(res == 1)
					break;							
				else
					continue;
			}			
			if(mysignal == SIGUSR1){
				//printf("Child: SIGUSR1 caught by receiver, morse code saved in CodeSequence holder\n");//debugging sign
				morse = morse<<1;
				morse+=1;//morse|=0x01;
			}
			if(mysignal == SIGUSR2){
				//printf("Child: SIGUSR2 caught by receiver, morse code saved in CodeSequence holder\n");//debugging sign
				morse = morse<<1;
			}
			if(mysignal == SIGALRM){
				fprintf(logfile,"Child: Character_divider caught by receiver, parse the MorseCode Sequence and write the character to file\n");
				fflush(logfile);
				char tmp = readmorse(morse);
				fputc(tmp,filetowrite);
				morse = 0x01;
			}
			if(mysignal == SIGTERM){
				fprintf(logfile,"Child: SIGTERM CAUGHT BY RECEIVER!CLOSING FILES AND PROGRAM EXIT NORMALLY!\n");
				fflush(logfile);//unnecessary since fclose will do the flush!!
				fclose(logfile);
				fclose(filetowrite);
				return;
			}
			if(mysignal == SIGCONT){
				fprintf(logfile,"Child: Encounter undefine character, check the character to be decoded next, file closed!\n");
				fflush(logfile);//unnecessary since fclose will do the flush!!
				fclose(logfile);
				fclose(filetowrite);
				return ;
			}				
		}
}
		

/******************THE MAIN PROGRAM*************************/
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
	int pipefdreceiver[2];
	
	assert(pipe(pipefdsender) == 0);
	sigpipebufsender = pipefdsender[1];//set for parent the sender to use

	assert(pipe(pipefdreceiver) == 0);
	sigpipebufreceiver = pipefdreceiver[1];//set for child the receiver(decoder) to use
	
	struct sigaction sigsender;//deal with "go" signal(SIGINT) from the receiver(child)
	sigemptyset(&sigsender.sa_mask);
	sigsender.sa_flags = 0;
	sigsender.sa_handler = sig_sender;//the handler name is sig_sender
	assert((sigaction(SIGINT,&sigsender,NULL))==0);
	

	struct sigaction sigreceiver;//deal with morse code
	sigemptyset(&sigreceiver.sa_mask);
	sigreceiver.sa_flags = 0;
	sigreceiver.sa_handler = sig_receiver;
	assert((sigaction(SIGUSR1,&sigreceiver,NULL))==0);//dit	
	assert((sigaction(SIGUSR2,&sigreceiver,NULL))==0);//dah
	assert((sigaction(SIGALRM,&sigreceiver,NULL))==0);//character divider
	assert((sigaction(SIGTERM,&sigreceiver,NULL))==0);//end of file, close file then.
	assert((sigaction(SIGCONT,&sigreceiver,NULL))==0);//notifying the receiver that an undefined character is encountered in the reader side,asking for close-file
/***************fork the process*****************/
	pid_t tpid;
	tpid = fork();
/**************the Parent process also the sender*******************/	
	if(tpid>0){
		close(pipefdreceiver[0]);
		close(pipefdreceiver[1]);//parent doesn't use pipefdreceiver as signal buffer, so cut the ties with pipefdreceiver in parent process
		senderprocess(argv[1],tpid,pipefdsender[0]);					
	}
/***************child process also the decoder************************/	
	if(tpid==0){
		close(pipefdsender[0]);
		close(pipefdsender[1]);
		pid_t ppid = getppid();
		decoderprocess(argv[1],ppid,pipefdreceiver[0]);
	}
	return 0;
}


