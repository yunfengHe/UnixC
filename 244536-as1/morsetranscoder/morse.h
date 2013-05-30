#ifndef MORSE_H
#define MORSE_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <strings.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>


void sendcode(int pipeid,int pipefdreaderside, char chara);//send the morse code sequence for a single character

void sendsignal(int pipeid, int num);// a kill() function wrapped inside, proves a troublesome design

char readmorse(char morse);//a funtion called by the decoder process, which translate morse code sequences into character. the argument is a code sequence holder,an 8 bits char

void checkstate(int pipefdreaderside);//checks if there's a signal in the pipe buffer, if so return, if not, block and wait.



#endif
