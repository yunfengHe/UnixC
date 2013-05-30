#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
/*in this program, each process(parent and child) use its own pipe to buffer signal. before fork, two pipes are made. in this method, signal handler just write signal to pipe whenever signal comes, this has helped prevent the possibility of signal loss in my sigwait or pause model, since signal may arrive just before sigwait and pause and get lost!

My previous test on sigwait proves that even with the help of mask, it's still unreliable, signal just get lost between the window time of function calls. This program is tested to be solid reliable!!*/

static int sigpipe = 0;
static void sig_usr(int signo){
	char ret = signo;
	write(sigpipe,&ret,1);
}

static int sigpipe1 = 0;
static void sig_parentsideint(int signo){
		char rett = signo;
		write(sigpipe1, &rett,1);
}


int main(void){
	
	int pipefd[2];//used by child
	int pipefd1[2];//used by parent

	assert(pipe(pipefd) == 0);
	sigpipe = pipefd[1];//set for child to use

	assert(pipe(pipefd1) == 0);
	sigpipe1 = pipefd1[1];//set for parent to use

	pid_t tpid;

	
	struct sigaction sig;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;
	sig.sa_handler = sig_usr;
	
	assert((sigaction(SIGUSR1, &sig,NULL))== 0);
	assert((sigaction(SIGUSR2, &sig,NULL)) == 0);//set handler for child(parent also but I will close pipe later,actually it doesn't matter if I close pipe or not)

	struct sigaction sig1;
	sigemptyset(&sig1.sa_mask);
	sig1.sa_flags = 0;
	sig1.sa_handler = sig_parentsideint;
	
	assert((sigaction(SIGINT, &sig1,NULL))== 0);

	tpid = fork();
	if (tpid>0){
		close(pipefd[0]);
		close(pipefd[1]);//parent use its pipefd1 to buffer signal(when I close both, then pipe just vanish in both processes??--answer is no, in my parent code, line 69 was SIGINT,that's result the child to block there, since it doesn't read pipefd1 at all!)
		
		int i=0;

		for(;;){
			char mysignal;
			int res = read(pipefd1[0],&mysignal,1);
			if (res == 1)
				break;
			else
				continue;		
		}

		for (i=0; i<10;i++){
			//kill(tpid, 30);this won't work, just use the macro instead of Integer
			kill(tpid, SIGUSR1);//SIGUSR1
			for(;;){
				char mysignal;
				int res = read(pipefd1[0],&mysignal,1);
				if (res == 1)
					break;
				else
					continue;		
			}
			kill(tpid,SIGUSR2);
			for(;;){
				char mysignal;
				int res = read(pipefd1[0],&mysignal,1);
				if (res == 1)
					break;
				else
					continue;		
			}
			
		}
		kill(tpid,SIGKILL);// kill the child at this point
		return;
	}

	if(tpid == 0){
	/*this is the child*/
		close(pipefd1[1]);
		close(pipefd1[0]);//child use pipefd to buffer signal

		pid_t ppid = getppid();
		
		for(;;){			
			kill(ppid,SIGINT);/*kill(getppid(),SIGINT);*/

			for(;;){
				char mysignal;
				int res = read(pipefd[0],&mysignal,1);
				if (res == 1){
					if (mysignal == SIGUSR1)
					       printf("received SIGUSR1\n");
        				else if (mysignal == SIGUSR2)
						printf("received SIGUSR2\n");			
					break;
					}
				else
					continue;
			}// use a loop to check pipe, to replace pause, would be better.						
		}
		
	}
	return 0;
}
		
	
	

