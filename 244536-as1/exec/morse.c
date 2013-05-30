#include "morse.h"
/*Limitations
1.upper or lower case letters will or be understood as lower case letters
2.(Has been sloved already Now.)some signs like () {} [] !  are not defined!!!!! can define new characters in sendcode function!
*/

void checkstate(int pipefdreaderside){
	for(;;){
		char mysignal;
		int res = read(pipefdreaderside,&mysignal,1);
		if (res == 1 )/*"&& mysignal==SIGINT"; not necessary as only SIGINT will be hold in the pipe by the handler*/
			break;
		else
			continue;		
	}
	return;
}

void sendsignal(int pipeid, int num){
	if(num == 30)
	kill(pipeid, SIGUSR1);// send the number of a signal to the process is not working, just use macro!!!!
	if(num == 31)
	kill(pipeid, SIGUSR2);
	if(num == 1)
	kill(pipeid, SIGCONT);
}


void sendcode(int pipeid,int pipefdreaderside, char chara){
/*pipeid here is essentially process id of the receiver!,shall be pid*/
	switch(chara){
	case 'a'://usrsignal1 == 30; usrs2 == 31----send the number of a signal to the process prove not working, just use macro!!!!	
		sendsignal(pipeid,30);// the receiver would read it and understand as '1'
		//*printf("sigUSR1 sent\n");debugging sign*//
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'b':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'c':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'd':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'e':
		sendsignal(pipeid,30);
		break;
	case 'f':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'g':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'h':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'i':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'j':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'k':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'l':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'm':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'n':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'o':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'p':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'q':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'r':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 's':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 't':
		sendsignal(pipeid,31);
		break;
	case 'u':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'v':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'w':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'x':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'y':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'z':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
/*UPPERCASR LETTERS*/
	case 'A'://usrsignal1 == 30; uss2 ==31
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'B':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'C':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'D':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'E':
		sendsignal(pipeid,30);
		break;
	case 'F':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'G':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'H':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'I':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'J':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'K':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'L':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'M':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'N':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'O':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'P':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'Q':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'R':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'S':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case 'T':
		sendsignal(pipeid,31);
		break;
	case 'U':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'V':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'W':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'X':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'Y':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case 'Z':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
/*digit*/
	case '0':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case '1':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case '2':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case '3':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case '4':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case '5':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case '6':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case '7':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case '8':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case '9':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
/*MARKS*/
	case '.':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case ',':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case ':':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case '?':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case '\''://for '
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case '-':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case '/':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case '"':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case '@':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case '=':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		break;
	case '\n':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		break;
	case ' ':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);		
		break;
/*************defined by myself recently*****************/	
	case ';':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);		
		break;	
	case '(':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);		
		break;	
	case ')':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);		
		break;
	case '!':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);		
		break;	
	case '[':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);		
		break;	
	case ']':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);		
		break;	
	case '{':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);		
		break;	
	case '}':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);		
		break;	

	case '_':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);		
		break;	
	case '+':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);		
		break;
	case '*':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);		
		break;
	case '&':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);		
		break;
	case '^':
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);		
		break;
	case '%':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);		
		break;
	case '$':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);		
		break;
	case '#':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);		
		break;
	case '<':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);		
		break;
	case '>':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);		
		break;
	case '\t':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);		
		break;
	case '\\':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);		
		break;
	case '|':
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,31);
		checkstate(pipefdreaderside);
		sendsignal(pipeid,30);		
		break;
	default:
		sendsignal(pipeid,1);/*case when the character is not recognized, tell the decoder to shut down*/
		exit(EXIT_FAILURE);
	}
	return;//successfully sent the morse code sequence for one character
}


/*************upper or lower case letters will all be understood as lower case letters*******************/
char readmorse(char morse){
	int tmp = morse;
	if(tmp == 6 )
		return 'a';
	else if(tmp == 23)//integer value for a character has been calculated manually, laborious work!
		return 'b';
	else if(tmp == 21)
		return 'c';
	else if(tmp == 11)
		return 'd';
	else if(tmp == 3)
		return 'e';
	else if(tmp == 29)
		return 'f';
	else if(tmp == 9)
		return 'g';
	else if(tmp == 31)
		return 'h';
	else if(tmp == 7)
		return 'i';
	else if(tmp == 24)
		return 'j';
	else if(tmp == 10)
		return 'k';
	else if(tmp == 27)
		return 'l';
	else if(tmp == 4)
		return 'm';
	else if(tmp == 5)
		return 'n';
	else if(tmp == 8)
		return 'o';
	else if(tmp == 25)
		return 'p';
	else if(tmp == 18)
		return 'q';
	else if(tmp == 13)
		return 'r';
	else if(tmp == 15)
		return 's';
	else if(tmp == 2)
		return 't';
	else if(tmp == 14)
		return 'u';
	else if(tmp == 30)
		return 'v';
	else if(tmp == 12)
		return 'w';
	else if(tmp == 22)
		return 'x';
	else if(tmp == 20)
		return 'y';
	else if(tmp == 19)
		return 'z';
	else if(tmp == 32)
		return '0';
	else if(tmp == 48)
		return '1';
	else if(tmp == 56)
		return '2';
	else if(tmp == 60)
		return '3';
	else if(tmp == 62)
		return '4';
	else if(tmp == 63)
		return '5';
	else if(tmp == 47)
		return '6';
	else if(tmp == 39)
		return '7';
	else if(tmp == 35)
		return '8';
	else if(tmp == 33)
		return '9';
	else if(tmp == 106)
		return '.';
	else if(tmp == 76)
		return ',';
	else if(tmp == 71)
		return ':';
	else if(tmp == 115)
		return '?';
	else if(tmp == 97)
		return '\'';
	else if(tmp == 94)
		return '-';
	else if(tmp == 45)
		return '/';
	else if(tmp == 109)
		return '"';
	else if(tmp == 101)
		return '@';
	else if(tmp == 46)
		return '=';
	else if(tmp == 127)
		return '\n';
	else if(tmp == 64)
		return ' ';
/*************defined by myself recently*****************/
	else if(tmp == 85)
		return ';';
	else if(tmp == 82)
		return '(';
	else if(tmp == 83)
		return ')';
	else if(tmp == 84)
		return '!';
	else if(tmp == 86)
		return '[';
	else if(tmp == 87)
		return ']';
	else if(tmp == 88)
		return '{';
	else if(tmp == 89)
		return '}';
	else if(tmp == 90)
		return '_';
	else if(tmp == 91)
		return '+';
	else if(tmp == 92)
		return '*';
	else if(tmp == 93)
		return '&';
	else if(tmp == 95)
		return '^';
	else if(tmp == 96)
		return '%';
	else if(tmp == 98)
		return '$';
	else if(tmp == 99)
		return '#';
	else if(tmp == 100)
		return '<';
	else if(tmp == 102)
		return '>';
	else if(tmp == 103)
		return '\t';
	else if(tmp == 104)
		return '\\';
	else if(tmp == 105)
		return '|';		
	else
		return ' ';//if not found, return ' '
}



		


