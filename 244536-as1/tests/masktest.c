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
	

int main(void){

	pid_t tpid;

	sigset_t newmask, oldmask, pendmask;/*oldmask just save the previous mask*/
	sigfillset(&newmask);
	sigprocmask(SIG_BLOCK,&newmask,NULL/*&oldmask*/);/*fillset后再block确实起作用，不过我的程序有信号丢失，试着解释下(because I didn't write sleep before sending signal, the child didn't unblock the mask until a lot of siganls has been sent and lost.After I write sleep there, I got every signal caught)*/
	
	sigemptyset(&newmask);/*<<<<<<<testable line!!>>>>>>>if I don't empty the set, then the unblock later would just unblock every signal, empty here and the unblock action will unblock only those who are addsetted to the set*/
	
	tpid = fork();
	if (tpid>0){
		/*this is the parent process*/
		int i=0;
		sleep(2);/*<<<<<<<testable line!!>>>>>>>avoid signal loss*/
		for (i=0; i<10;i++){
			kill(tpid, SIGINT);
			sleep(1);/*<<<<<<<testable line!!>>>>>>>avoid race condition, when handling one signal, another just came, then the signal handler is interrupted by SIGUSR1 and never get a chance to handle anything*/
			kill(tpid,SIGUSR1);
			sleep(1);
		}
		kill(tpid,SIGKILL);// kill the child before quit,otherwise an orphan will be created!
		return;
	}
	if(tpid == 0){
	/*this is the child*/
		signal(SIGINT,sig_int);
		sigaddset(&newmask, SIGINT);/*<<<<<<<testable line!!>>>>>>>*/
		sigaddset(&newmask, SIGUSR1);/*<<<<<<<testable line!!>>>>>>>if I don't enable this, then SIGUSR1 won't be caught, just blocked!!*/
		sigprocmask(SIG_UNBLOCK, &newmask, NULL);/*<<<<<<<testable line!!>>>>>>>*/
		for(;;){
			pause();
		}
		
	}
	return 0;
}
		
	
	

