#ifndef MORSE_H
#define MORSE_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <strings.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

/*the function must decide whether it's ok to send the siganl,
that is, to wait for the acknowledgement from the receiver*/

void sendcode(int pipeid,int pipefdreaderside, char chara);//send the morse code sequence for a single character

void sendsignal(int pipeid, int num);// a kill() function wrapped inside, proves a troublesome design

char readmorse(char morse);

void checkstate(int pipefdreaderside);



#endif
