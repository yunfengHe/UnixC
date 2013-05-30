#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include "decoder.h"

static int sigpipebufreceiver = 0;
static void sig_receiver(int signo){	
		char ret = signo;
		write(sigpipebufreceiver,&ret,1);
}

/**************Receiver task that giving 'go' signal to the sender and wait to receive signal sets for certain characters, parse it, write to file********************/
void decoderprocess(char *filename,int senderpid, int pipefdreaderside){
/**********open file for logging***************/
	FILE* logfile = NULL;//don't have to use filelock since two processes access the file in order,just fflush after each fprintf()
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
			exit(EXIT_FAILURE); //don't forget to register atexit
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
				//fflush(logfile);//unnecessary since fclose will do the flush!!
				fclose(logfile);
				fclose(filetowrite);
				return;
			}
			if(mysignal == SIGCONT){
				fprintf(logfile,"Child: Encounter undefine character, check the character to be decoded next, files closed!\n");
				//fflush(logfile);//unnecessary since fclose will do the flush!!
				fclose(logfile);
				fclose(filetowrite);
				return ;
			}				
		}
}


int main(int argc, char* argv[])
{
	int pipefdreceiver[2];

	assert(pipe(pipefdreceiver) == 0);
	sigpipebufreceiver = pipefdreceiver[1];//set for child the receiver(decoder) to use

	struct sigaction sigreceiver;//deal with morse code
	sigemptyset(&sigreceiver.sa_mask);
	sigreceiver.sa_flags = 0;
	sigreceiver.sa_handler = sig_receiver;
	assert((sigaction(SIGUSR1,&sigreceiver,NULL))==0);//dit	
	assert((sigaction(SIGUSR2,&sigreceiver,NULL))==0);//dah
	assert((sigaction(SIGALRM,&sigreceiver,NULL))==0);//character divider
	assert((sigaction(SIGTERM,&sigreceiver,NULL))==0);//end of file, close file then.
	assert((sigaction(SIGCONT,&sigreceiver,NULL))==0);//notifying the receiver that an undefined character is encountered in the reader side,asking for close-file	

	pid_t ppid = getppid();//This shall work since after exec, the process still hold the relationship with the parent process.
	decoderprocess(argv[1],ppid,pipefdreceiver[0]);
	return 0;
}

