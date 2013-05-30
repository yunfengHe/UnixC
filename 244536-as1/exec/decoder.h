#ifndef DECODER_H
#define DECODER_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <strings.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>


char readmorse(char morse);

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



#endif
