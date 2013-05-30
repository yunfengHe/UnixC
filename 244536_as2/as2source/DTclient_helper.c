/*Helper function, so that the user can open multiple files at once,The function would make use of DTclient
Usage: ./DTclient_helper [filename1] [filename2]...[filenamek]...*/
#include "DTlib.h"

//dealing with signal is dangerous, this signal counter may not work because if more than one SIGCHLD arrives before the handler return(if only one, sigaction helps block it, if two, there's nothing sigaction can do), signal get lost anyway, there's no underlayer buffer to buffer more than one instance of the same signal in sigaction.

/*set handler for SIGCHLD*/
/*static int sigchildcount = 0;
static void sig_receiver(int signo){
		//char ret = signo;
		if (signo == SIGCHLD)	
		sigchildcount += 1;
			
}
*/

int main(int argc, char* argv[])
{
	if (argc < 2){
		printf("\nUsage: ./DTclient_helper [filename1] [filename2]...[filenamek]... \n");
		return 0;
	}

/*set sigaction to deal with SIGCHILD*/
/*	struct sigaction sigreceiver;
	sigemptyset(&sigreceiver.sa_mask);
	sigreceiver.sa_flags = 0;
	sigreceiver.sa_handler = sig_receiver;
	assert((sigaction(SIGCHLD,&sigreceiver,NULL))==0);
*/

/*fork and call exec()*/	
	int i;	
	for(i=1; i<argc; i++){
		pid_t tmpchild = fork();
		if(tmpchild == 0){
			//char *argv_execv[]= {"DTclient", argv[i],NULL};//this gives warning, argv[i] is only known at run time!!
			char *argvtmp = strcpy(malloc(strlen(argv[i])+1),argv[i]);
			char *argv_execv[3];
			argv_execv[0] = "DTclient";			
			argv_execv[1] = argvtmp;
			argv_execv[2] = NULL; 			
			if(execv("DTclient",argv_execv)<0){
				perror("Error on execv");
			}
			free(argvtmp);
		}
	}
/*wait for all child to return before return*/
/*	while(1){
		sleep(1);
		if(sigchildcount == (argc-1))
		break;
	}
*/
	pthread_exit(NULL);// in this way the main thread returns without affecting other running child processes.
}

	

		

