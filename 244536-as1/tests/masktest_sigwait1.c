#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
/*测试sigpromask和sigfillset, sigemptyset 的用法*/
/*In this Program, the function of sigprocmask and sigemptyset, sigfillset, sigaddset are properly tested.(This test program is done and passed test. just use gcc to compile.)*/

static void sig_int(int);

static void sig_int(int signo){
	signal(SIGINT,sig_int);
	signal(SIGUSR1,sig_int);
	if(signo ==SIGINT)
		printf("caught SIGINT\n");					
	if(signo==SIGUSR1)
		printf("caught SIGUSR1\n");					
	return;
}

static void sig_pint(int);

static void sig_pint(int signo){
	signal(SIGINT,sig_pint);
	
	if(signo ==SIGINT)						
		return;
}	

int main(void){

	sigset_t setforparent;
	sigemptyset(&setforparent);
	sigaddset(&setforparent, SIGINT);/*only SIGINT is in the set for sigwait to use; for solution 2*/
	int signum = 0;/*number of signals caught, cannot be NULL, otherwise there would be segmentation fault*/
	

	pid_t tpid;
	sigset_t newmask, oldmask, pendmask;/*oldmask just save the previous mask*/
	sigfillset(&newmask);
	//sigdelset(&newmask,SIGINT);/*solution 1*/
	sigprocmask(SIG_BLOCK,&newmask,NULL/*&oldmask*/);/*fillset后再block确实起作用，不过我的程序有信号丢失，试着解释下(because I didn't write sleep before sending signal, the child didn't unblock the mask until a lot of siganls has been sent and lost.After I write sleep there, I got every signal caught)*/
	
	sigemptyset(&newmask);/*<<<<<<<testable line!!>>>>>>>if I don't empty the set, then the unblock later would just unblock every signal, empty here and the unblock action will unblock only those who are addsetted to the set*/
	
	tpid = fork();
	if (tpid>0){
		//signal(SIGINT,sig_pint);/*solution 1*/
		
		sigprocmask(SIG_BLOCK,&setforparent,NULL);//solution 2
		/*this is the parent process*/
		int i=0;
		//pause();/*solution 1*/
		sigwait(&setforparent, &signum);//solution 2
		//sleep(2);/*<<<<<<<testable line!!>>>>>>>avoid signal loss*/
		for (i=0; i<10;i++){
			kill(tpid, SIGINT);
			//sleep(1);/*<<<<<<<testable line!!>>>>>>>avoid race condition, when handling one signal, another just came, then the signal handler is interrupted by SIGUSR1 and never get a chance to handle anything*/
			//pause();/*solution 1*/
			sigwait(&setforparent, &signum);//solution 2
			kill(tpid,SIGUSR1);
			//sleep(1);
			//pause();/*solution 1*/
			sigwait(&setforparent, &signum);//only wait for certain signal, when sigchild arrives, shall be able to terminate//solution 2
		}
		kill(tpid,SIGKILL);
		return;
	}
	if(tpid == 0){
	/*this is the child*/
		signal(SIGINT,sig_int);
		sigaddset(&newmask, SIGINT);/*<<<<<<<testable line!!>>>>>>>*/
		sigaddset(&newmask, SIGUSR1);/*<<<<<<<testable line!!>>>>>>>if I don't enable this, then SIGUSR1 won't be caught, just blocked!!*/
		sigprocmask(SIG_UNBLOCK, &newmask, NULL);/*<<<<<<<testable line!!>>>>>>>*/
		pid_t ppid = getppid();
		
		for(;;){			
			kill(ppid,SIGINT);/*kill(getppid(),SIGINT);*/
			pause();
			// use a loop to check pipe, to replace pause, would be better.						
		}
		
	}
	return 0;
}
		
	
	

