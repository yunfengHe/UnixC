#include <stdio.h>

int main(){
	char morse = 0x01;
	morse = morse<<1;
	morse |=0x01;
	int new = morse;
	printf("the number of morse is %u\n",morse);
}
